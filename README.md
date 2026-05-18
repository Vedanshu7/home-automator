# home-automator

> Control your home appliances from your Android phone over WiFi.

`home-automator` is an IoT project that pairs an Android app with Arduino firmware to remotely control 4 relay outputs (lights, fans, AC, etc.) over a local WiFi network. Commands are sent over TCP sockets via an ESP8266/ESP-01 WiFi module.

## Features

- Control 4 independent relay outputs from your phone
- Real-time status feedback
- Scheduled on/off using the Arduino RTC module (DS3231)
- Runs on a local WiFi network — no cloud dependency

## Hardware Required

| Component | Qty |
|-----------|-----|
| Arduino Uno / Mega | 1 |
| ESP8266 / ESP-01 WiFi module | 1 |
| DS3231 RTC module | 1 |
| 4-channel relay module | 1 |
| 5V power supply | 1 |
| Jumper wires | - |

### Pin Connections

| Arduino Pin | Connected To |
|-------------|-------------|
| RX (pin 12) | ESP8266 TX |
| TX (pin 13) | ESP8266 RX |
| A0–A3 | Relay IN1–IN4 |
| SDA / SCL | DS3231 SDA / SCL |

## Tech Stack

- **Firmware:** C++ (Arduino)
- **Mobile App:** Java (Android)
- **Communication:** TCP socket over WiFi (ESP8266 AT commands)
- **Scheduling:** DS3231 RTC

## Setup

### 1. Flash the Arduino

Open `src/arduino/Home_Auotmation.ino` in Arduino IDE, install the `Sodaq_DS3231` library, and upload to your board.

### 2. Build the Android App

Open `src/android/HomeAutomation/` in Android Studio (Android Studio Hedgehog or later recommended). Build and install on your device.

### 3. Configure WiFi

The ESP8266 is configured as an Access Point (AP mode). Connect your phone to the ESP8266 WiFi network, then use the app to send commands.

## Project Structure

```
home-automator/
├── src/
│   ├── android/HomeAutomation/   # Android app (Java + Gradle)
│   └── arduino/Home_Auotmation/  # Arduino firmware (.ino)
├── docs/
│   └── circuit-diagram.md        # Wiring instructions
└── README.md
```

## License

MIT
