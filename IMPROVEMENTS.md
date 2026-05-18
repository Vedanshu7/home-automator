# home-automator — Improvement Roadmap

## Protocol

- **MQTT** — replace raw TCP sockets with MQTT (Mosquitto broker) for reliable pub/sub messaging and reconnection handling
- **ESP32 support** — migrate from ESP8266 to ESP32 for dual-core processing, BLE, and faster WiFi
- **OTA firmware updates** — update Arduino firmware over-the-air via WiFi

## App

- **React Native** — rewrite Android app in React Native for cross-platform (iOS + Android) support
- **Voice control** — integrate Google Assistant / Alexa routines via IFTTT webhooks
- **Dashboard** — real-time device status tiles, power consumption graphs
- **Push notifications** — alert when a device has been on for too long

## Infrastructure

- **MQTT broker on Raspberry Pi** — run Mosquitto locally for home server mode
- **Cloud mode** — optional HiveMQ cloud broker for remote control outside home network
- **Home Assistant integration** — expose devices as Home Assistant entities via MQTT discovery

## Hardware

- **Energy monitoring** — add PZEM-004T power meter to track consumption per relay
- **Temperature/humidity sensor** — DHT22 for environment monitoring
- **IR blaster** — control TVs and ACs via infrared alongside relay control
