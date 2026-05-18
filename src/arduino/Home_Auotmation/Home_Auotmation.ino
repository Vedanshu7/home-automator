/*
 * home-automator firmware.
 *
 * Controls 4 relay outputs (A0–A3) via ESP8266 WiFi + DS3231 RTC.
 *
 * Immediate command protocol (2-char codes from Android app):
 *   "11" / "55" → relay 1 ON / OFF.
 *   "15" / "60" → relay 2 ON / OFF.
 *   "44" / "77" → relay 3 ON / OFF.
 *   "95" / "88" → relay 4 ON / OFF.
 *
 * Schedule packet (16-char string):
 *   [0–3]   relay mask — non-'0' at position i means relay (i+1) is included,
 *           e.g. "1034" selects relays 1, 3, and 4.
 *   [4–5]   target hour (2-digit, 24 h).
 *   [6–7]   target minute.
 *   [8–9]   unused.
 *   [10]    delay hours ("2", "3", or "4").
 *   [11]    unused.
 *   [12]    relay count.
 *   [13]    unused.
 *   [14–15] action code — '6' = schedule ON, '9' = schedule OFF.
 *
 * Relay wiring: active-LOW (HIGH = OFF, LOW = ON).
 */

#include <SoftwareSerial.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"

// ============================================================
// Pin definitions.
// ============================================================

// Four relay output pins mapped to analog pins used as digital outputs.
static const uint8_t RELAY_PINS[] = { A0, A1, A2, A3 };
static const uint8_t NUM_RELAYS   = sizeof(RELAY_PINS);

// RX pin 12, TX pin 13.
SoftwareSerial esp8266(12, 13);

// ============================================================
// Schedule state.
// ============================================================

struct Schedule {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    // Bit i set means RELAY_PINS[i] fires when this schedule triggers.
    uint8_t relayMask;
    // Flag cleared after the schedule fires to prevent repeat triggering.
    bool active;
};

// One schedule for turning relays ON and one for turning them OFF.
static Schedule onSchedule  = {};
static Schedule offSchedule = {};

// Last seen RTC epoch; used to detect second boundaries without calling rtc.now() twice.
static uint32_t lastEpoch = 0;

// ============================================================
// Relay control helpers.
// ============================================================

// Toggle every relay whose bit is set in mask.
static void applyRelays(uint8_t mask, uint8_t state) {
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        if (mask & (1 << i)) {
            digitalWrite(RELAY_PINS[i], state);
        }
    }
}

// Convert a 4-char relay string (e.g. "1034") to a 4-bit mask.
// A non-'0' character at position i means relay (i+1) is selected.
static uint8_t parseMask(const String& s) {
    uint8_t mask = 0;
    for (uint8_t i = 0; i < 4 && i < (uint8_t)s.length(); i++) {
        if (s[i] != '0') mask |= (1 << i);
    }
    return mask;
}

// ============================================================
// Immediate command dispatch table.
// ============================================================

// Maps 2-char command codes received from the app to a relay index and output state.
struct Cmd { const char* code; uint8_t relay; uint8_t state; };

static const Cmd COMMANDS[] = {
    { "11", 0, HIGH }, { "55", 0, LOW  },
    { "15", 1, HIGH }, { "60", 1, LOW  },
    { "44", 2, HIGH }, { "77", 2, LOW  },
    { "95", 3, HIGH }, { "88", 3, LOW  },
};

// Derived at compile time so adding entries to COMMANDS does not require a manual update.
static const uint8_t CMD_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

// Walk the table and apply the first matching command.
static void handleCommand(const String& cmd) {
    for (uint8_t i = 0; i < CMD_COUNT; i++) {
        if (cmd == COMMANDS[i].code) {
            digitalWrite(RELAY_PINS[COMMANDS[i].relay], COMMANDS[i].state);
            return;
        }
    }
}

// ============================================================
// ESP packet extraction.
// ============================================================

// Strip non-digit characters from the raw ESP packet and return the payload.
// Raw packets carry connection metadata around the actual data digits, so only
// the digit run at a known offset is relevant.
static String extractPayload(const String& raw) {
    String digits;
    for (uint16_t i = 0; i < raw.length(); i++) {
        if (raw[i] >= '0' && raw[i] <= '9') digits += raw[i];
    }
    // 5-digit run: metadata (2) + 2-char command (2) + checksum (1).
    if (digits.length() == 5)  return digits.substring(2, 4);
    // 20-digit run: metadata (3) + 16-char schedule (16) + checksum (1).
    if (digits.length() == 20) return digits.substring(3, 19);
    // Already a clean 16-char schedule with no wrapper.
    if (digits.length() == 16) return digits;
    return "";
}

// ============================================================
// Scheduled action parser.
// ============================================================

// Parse a 16-char schedule string and register an ON or OFF schedule.
// Also applies the relay state immediately so the user gets instant feedback.
static void parseSchedule(const String& s) {
    if (s.length() != 16) return;
    // Decode all fields from fixed character positions.
    uint8_t mask      = parseMask(s.substring(0, 4));
    int     targetHr  = s.substring(4, 6).toInt();
    int     targetMin = s.substring(6, 8).toInt();
    int     delayHrs  = s.substring(10, 11).toInt();
    bool    isOn      = (s[14] == '6');
    bool    isOff     = (s[14] == '9');
    if (!isOn && !isOff) return;
    // Read current time to calculate the absolute fire time.
    DateTime now = rtc.now();
    int curHr  = now.hour();
    int curMin = now.minute();
    int curSec = now.second();
    int hrDiff  = targetHr  - curHr;
    int minDiff = targetMin - curMin;
    // Determine when the schedule should fire.
    uint8_t fireHr  = curHr;
    uint8_t fireMin = curMin;
    uint8_t fireSec = curSec;
    if (hrDiff == delayHrs) {
        // Normal case: target is exactly delayHrs hours away.
        fireHr = curHr + delayHrs;
    } else if (delayHrs >= 3 && hrDiff == 0 && (minDiff == 2 || minDiff == 3)) {
        // Sub-hour scheduling: fire 2 minutes from now when target is within the same hour.
        fireMin = curMin + 2;
    } else {
        // Timing does not match expected window — ignore packet.
        return;
    }
    Schedule sched = { fireHr, fireMin, fireSec, mask, true };
    if (isOn) {
        onSchedule = sched;
        applyRelays(mask, LOW);
    } else {
        offSchedule = sched;
        applyRelays(mask, HIGH);
    }
}

// ============================================================
// Arduino setup and main loop.
// ============================================================

void setup() {
    // Configure every relay pin as output and start in the OFF state.
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        pinMode(RELAY_PINS[i], OUTPUT);
        // Active-LOW relay: HIGH = OFF.
        digitalWrite(RELAY_PINS[i], HIGH);
    }
    // Open debug serial port and ESP8266 software serial.
    Serial.begin(9600);
    esp8266.begin(115200);
    // Configure ESP8266 as a WiFi access point (mode 2).
    esp8266.println("AT+CWMODE=2"); delay(200);
    // Allow multiple simultaneous TCP connections.
    esp8266.println("AT+CIPMUX=1"); delay(200);
    // Start TCP server on port 333.
    esp8266.println("AT+CIPSERVER=1,333");
    Wire.begin();
    rtc.begin();
}

void loop() {
    // Small yield to prevent tight-looping while waiting for serial data.
    delay(10);
    // ============================================================
    // Read and dispatch incoming WiFi packets.
    // ============================================================
    if (esp8266.available()) {
        String raw;
        while (esp8266.available()) raw += (char)esp8266.read();
        String payload = extractPayload(raw);
        Serial.println(payload);
        if (payload.length() == 2)       handleCommand(payload);
        else if (payload.length() == 16) parseSchedule(payload);
    }
    // ============================================================
    // Fire pending scheduled relay actions (runs once per RTC second).
    // ============================================================
    DateTime now = rtc.now();
    uint32_t ts  = now.getEpoch();
    if (ts != lastEpoch) {
        lastEpoch = ts;
        uint8_t h = now.hour(), m = now.minute(), s = now.second();
        if (onSchedule.active
                && h == onSchedule.hour
                && m == onSchedule.minute
                && s == onSchedule.second) {
            applyRelays(onSchedule.relayMask, LOW);
            // Clear the schedule so it does not fire again next second.
            onSchedule.active = false;
        }
        if (offSchedule.active
                && h == offSchedule.hour
                && m == offSchedule.minute
                && s == offSchedule.second) {
            applyRelays(offSchedule.relayMask, HIGH);
            // Clear the schedule so it does not fire again next second.
            offSchedule.active = false;
        }
    }
    // ============================================================
    // Forward Serial input to ESP for AT command passthrough.
    // ============================================================
    if (Serial.available()) {
        // Short delay lets the full command arrive before reading.
        delay(10);
        String cmd;
        while (Serial.available()) cmd += (char)Serial.read();
        esp8266.println(cmd);
    }
}
