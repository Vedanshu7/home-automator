/*
 * home-automator firmware
 *
 * Controls 4 relay outputs (A0–A3) via ESP8266 WiFi + DS3231 RTC.
 *
 * Immediate command protocol (2-char codes from Android app):
 *   "11" / "55" → relay 1 ON / OFF
 *   "15" / "60" → relay 2 ON / OFF
 *   "44" / "77" → relay 3 ON / OFF
 *   "95" / "88" → relay 4 ON / OFF
 *
 * Schedule packet (16-char string):
 *   [0–3]   relay mask  — non-'0' at position i means relay (i+1) is included
 *                         e.g. "1034" → relays 1, 3, 4
 *   [4–5]   target hour (2-digit, 24 h)
 *   [6–7]   target minute
 *   [8–9]   unused
 *   [10]    delay hours ("2", "3", or "4")
 *   [11]    unused
 *   [12]    relay count
 *   [13]    unused
 *   [14–15] action code — '6' = schedule ON, '9' = schedule OFF
 *
 * Relay wiring: active-LOW (HIGH = OFF, LOW = ON).
 */

#include <SoftwareSerial.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"

// ── Pins ──────────────────────────────────────────────────────────────────────
static const uint8_t RELAY_PINS[] = { A0, A1, A2, A3 };
static const uint8_t NUM_RELAYS   = sizeof(RELAY_PINS);

SoftwareSerial esp8266(12, 13); // RX, TX

// ── Schedule ──────────────────────────────────────────────────────────────────
struct Schedule {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t relayMask; // bit i set → RELAY_PINS[i] fires
    bool    active;
};

static Schedule onSchedule  = {};
static Schedule offSchedule = {};
static uint32_t lastEpoch   = 0;

// ── Relay helpers ─────────────────────────────────────────────────────────────

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

// ── Immediate command table ───────────────────────────────────────────────────
struct Cmd { const char* code; uint8_t relay; uint8_t state; };

static const Cmd COMMANDS[] = {
    { "11", 0, HIGH }, { "55", 0, LOW  },
    { "15", 1, HIGH }, { "60", 1, LOW  },
    { "44", 2, HIGH }, { "77", 2, LOW  },
    { "95", 3, HIGH }, { "88", 3, LOW  },
};
static const uint8_t CMD_COUNT = sizeof(COMMANDS) / sizeof(COMMANDS[0]);

static void handleCommand(const String& cmd) {
    for (uint8_t i = 0; i < CMD_COUNT; i++) {
        if (cmd == COMMANDS[i].code) {
            digitalWrite(RELAY_PINS[COMMANDS[i].relay], COMMANDS[i].state);
            return;
        }
    }
}

// ── Packet extraction ─────────────────────────────────────────────────────────
// Strip non-digit characters from the raw ESP packet and return the payload.
static String extractPayload(const String& raw) {
    String digits;
    for (uint16_t i = 0; i < raw.length(); i++) {
        if (raw[i] >= '0' && raw[i] <= '9') digits += raw[i];
    }
    if (digits.length() == 5)  return digits.substring(2, 4);   // 2-char command
    if (digits.length() == 20) return digits.substring(3, 19);  // 16-char schedule
    if (digits.length() == 16) return digits;
    return "";
}

// ── Schedule parser ───────────────────────────────────────────────────────────
static void parseSchedule(const String& s) {
    if (s.length() != 16) return;

    uint8_t mask      = parseMask(s.substring(0, 4));
    int     targetHr  = s.substring(4, 6).toInt();
    int     targetMin = s.substring(6, 8).toInt();
    int     delayHrs  = s.substring(10, 11).toInt();
    bool    isOn      = (s[14] == '6');
    bool    isOff     = (s[14] == '9');

    if (!isOn && !isOff) return;

    DateTime now = rtc.now();
    int curHr  = now.hour();
    int curMin = now.minute();
    int curSec = now.second();

    int hrDiff  = targetHr  - curHr;
    int minDiff = targetMin - curMin;

    uint8_t fireHr  = curHr;
    uint8_t fireMin = curMin;
    uint8_t fireSec = curSec;

    if (hrDiff == delayHrs) {
        fireHr = curHr + delayHrs;
    } else if (delayHrs >= 3 && hrDiff == 0 && (minDiff == 2 || minDiff == 3)) {
        // Sub-hour scheduling: fire 2 minutes from now when target is within the same hour.
        fireMin = curMin + 2;
    } else {
        return; // timing doesn't match expected window — ignore
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

// ── Arduino lifecycle ─────────────────────────────────────────────────────────
void setup() {
    for (uint8_t i = 0; i < NUM_RELAYS; i++) {
        pinMode(RELAY_PINS[i], OUTPUT);
        digitalWrite(RELAY_PINS[i], HIGH); // start all relays OFF
    }

    Serial.begin(9600);
    esp8266.begin(115200);
    esp8266.println("AT+CWMODE=2"); delay(200);
    esp8266.println("AT+CIPMUX=1"); delay(200);
    esp8266.println("AT+CIPSERVER=1,333");

    Wire.begin();
    rtc.begin();
}

void loop() {
    delay(10);

    // ── WiFi input ────────────────────────────────────────────────────────────
    if (esp8266.available()) {
        String raw;
        while (esp8266.available()) raw += (char)esp8266.read();

        String payload = extractPayload(raw);
        Serial.println(payload);

        if (payload.length() == 2)       handleCommand(payload);
        else if (payload.length() == 16) parseSchedule(payload);
    }

    // ── Schedule tick (runs once per RTC second) ──────────────────────────────
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
            onSchedule.active = false;
        }
        if (offSchedule.active
                && h == offSchedule.hour
                && m == offSchedule.minute
                && s == offSchedule.second) {
            applyRelays(offSchedule.relayMask, HIGH);
            offSchedule.active = false;
        }
    }

    // ── Serial passthrough to ESP (AT command mode) ───────────────────────────
    if (Serial.available()) {
        delay(10);
        String cmd;
        while (Serial.available()) cmd += (char)Serial.read();
        esp8266.println(cmd);
    }
}
