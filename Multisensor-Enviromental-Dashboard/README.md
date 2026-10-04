# Multi-Sensor Environmental Dashboard (ESP32 + Blynk)

An IoT-based environmental monitoring system built with an ESP32 microcontroller. Four sensors — DHT22, LDR, PIR, and HC-SR04 — track temperature, humidity, light intensity, motion, and proximity in real time, with all data displayed on a live Blynk IoT dashboard. An event-driven alert system sends push notifications when motion is detected or an object gets too close. The project is developed and simulated entirely in the Wokwi simulator, running inside VS Code with PlatformIO, so no physical hardware is required.

## How It Works

Sensors (DHT22, LDR, PIR, HC-SR04) → ESP32 → WiFi → Blynk Cloud → Mobile app / Web dashboard + Push notifications

1. The ESP32 reads all four sensors every 2 seconds.
2. Temperature, humidity, light level, motion status, and distance are sent to the Blynk Cloud.
3. The Blynk dashboard displays all values live on gauges, an LED indicator, and a history chart.
4. When motion is newly detected, or an object comes closer than the set threshold, the ESP32 triggers a Blynk event, which sends a push notification to the phone.

## Components Used

| Component | Purpose |
|---|---|
| ESP32 | Main controller with built-in WiFi |
| DHT22 | Measures temperature and humidity |
| LDR (Photoresistor) | Measures ambient light level |
| PIR Motion Sensor | Detects movement |
| HC-SR04 | Measures distance (ultrasonic) |
| Blynk IoT | Mobile app and cloud dashboard, with push notification alerts |
| Wokwi | Online simulator (replaces real hardware) |
| PlatformIO (VS Code) | Development environment used to build and run the simulation |

## Circuit Connections

### DHT22 (Temperature + Humidity)

| DHT22 Pin | Connect To |
|---|---|
| VCC | ESP32 3V3 |
| SDA | ESP32 GPIO 15 |
| GND | ESP32 GND |

### LDR (Light Sensor)

| LDR Pin | Connect To |
|---|---|
| VCC | ESP32 3V3 |
| AOUT | ESP32 GPIO 34 |
| GND | ESP32 GND |

### PIR (Motion Sensor)

| PIR Pin | Connect To |
|---|---|
| VCC | ESP32 5V |
| OUT | ESP32 GPIO 27 |
| GND | ESP32 GND |

### HC-SR04 (Ultrasonic/Distance Sensor)

| HC-SR04 Pin | Connect To |
|---|---|
| VCC | ESP32 5V |
| TRIG | ESP32 GPIO 18 |
| ECHO | ESP32 GPIO 19 |
| GND | ESP32 GND |

### Combined Pin Summary

| Sensor | Signal Pin | ESP32 Pin | Power | Ground |
|---|---|---|---|---|
| DHT22 | SDA | GPIO 15 | 3V3 | GND |
| LDR | AOUT | GPIO 34 | 3V3 | GND |
| PIR | OUT | GPIO 27 | 5V | GND |
| HC-SR04 | TRIG | GPIO 18 | 5V | GND |
| HC-SR04 | ECHO | GPIO 19 | 5V | GND |

**Circuit image:** image ki place ha

## Blynk Setup

1. Create a template named `Multi-Sensor Dashboard` (Hardware: ESP32, Connection: WiFi).
2. Add five datastreams:
   - `Temperature` — V0, Double
   - `Humidity` — V1, Double
   - `Light Level` — V2, Integer
   - `Motion Status` — V3, Integer
   - `Distance` — V4, Double
3. Add two Events (for push notification alerts):
   - `motion_alert` — "Motion Detected"
   - `object_close` — "Object Too Close"
4. Add widgets to the dashboard:
   - Gauge for Temperature (V0), Humidity (V1), Light Level (V2), Distance (V4)
   - LED widget for Motion Status (V3)
   - Chart widget linking the datastreams, for historical trends
5. Create a device from the template and copy the Template ID, Template Name, and Auth Token.

## How to Run

1. Open the project folder in VS Code with the PlatformIO and Wokwi Simulator extensions installed.
2. Open `platformio.ini` and confirm the required libraries are listed (Blynk, DHT sensor library).
3. Open `src/main.cpp` and replace the three placeholder Blynk values with your own.
4. Build the project (PlatformIO: Build).
5. Run the simulation (Command Palette → `Wokwi: Start Simulator`).
6. In Wokwi, trigger the PIR sensor and move the HC-SR04's distance slider closer than 10 cm to test the alerts.
7. Watch the Serial Monitor for live sensor readings and alert messages, and check the Blynk dashboard and phone for push notifications.

**Working screenshot:** image ki place ha

## Code Overview

| Part | What It Does |
|---|---|
| `readDistanceCM()` | Sends a trigger pulse and measures the echo return time to calculate distance |
| `digitalRead(PIR_PIN)` | Reads the PIR sensor's motion status |
| `Blynk.logEvent(...)` | Sends an event to Blynk, which triggers a push notification |
| `lastMotionState` / `distanceAlertSent` | Prevent the same alert from being sent repeatedly while the condition stays true |
| `timer.setInterval(2000L, readAllSensors)` | Repeats the full sensor read-and-send cycle every 2 seconds |

## Tools and Technologies

ESP32, Arduino C/C++, Blynk IoT, Wokwi Simulator, PlatformIO, VS Code

## Author

Danish
GitHub: [Danish3431-code](https://github.com/Danish3431-code)
