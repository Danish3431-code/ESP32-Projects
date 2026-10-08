# Smart Automation & Monitoring Hub

An ESP32 automation and monitoring project, fully simulated in **Wokwi**. It reads four sensors, switches a fan and a light automatically, sends the data to **Blynk** (web dashboard and phone alerts) and publishes it over **MQTT** (HiveMQ public broker) to a **Node-RED** dashboard. No physical hardware is required.

## Features

- Reads temperature and humidity (DHT22), light level (LDR), motion (PIR) and distance (HC-SR04)
- Automatic fan control: ON when the temperature is above 30 °C
- Automatic light control: ON when it is dark (LDR reading above 3000) and motion is detected
- Blynk web dashboard with gauges, LED widgets and charts
- Blynk push alerts: `motion_alert` (new motion) and `object_close` (object closer than 10 cm)
- MQTT publishing of seven topics every 2 seconds
- Node-RED dashboard with live gauges

## How it works

```
Wokwi (ESP32 + 4 sensors + 2 LEDs)
   |-- WiFi "Wokwi-GUEST" --> Internet
   |-- Blynk Cloud        --> Blynk dashboard + phone alerts
   |-- HiveMQ broker (broker.hivemq.com:1883) --> Node-RED dashboard
```

Node-RED and Blynk are not connected to each other. Both receive their data directly from the ESP32.

## Components (all simulated in Wokwi)

| Component | Purpose |
|---|---|
| ESP32 DevKit C V4 | Microcontroller |
| DHT22 | Temperature and humidity |
| LDR module | Light level |
| PIR motion sensor | Motion detection |
| HC-SR04 | Distance |
| Blue LED + 220 Ω resistor | Fan (relay simulation) |
| Yellow LED + 220 Ω resistor | Light (relay simulation) |

## Pin mapping

| Component | Component pin | ESP32 pin |
|---|---|---|
| DHT22 | VCC / SDA / GND | 3V3 / GPIO 15 / GND |
| LDR module | VCC / AO / GND | 3V3 / GPIO 34 / GND |
| PIR sensor | VCC / OUT / GND | 5V / GPIO 27 / GND |
| HC-SR04 | VCC / TRIG / ECHO / GND | 5V / GPIO 18 / GPIO 19 / GND |
| Fan LED (blue) | via 220 Ω resistor | GPIO 26 |
| Light LED (yellow) | via 220 Ω resistor | GPIO 25 |

The complete wiring is defined in `diagram.json`.

## Project structure

```
smart-automation-hub/
├── platformio.ini
├── wokwi.toml
├── diagram.json
├── src/
│   └── main.cpp
└── README.md
```

## Getting started

### Prerequisites

- [VS Code](https://code.visualstudio.com/) with the **PlatformIO IDE** and **Wokwi Simulator** extensions
- A Wokwi free license (`Ctrl+Shift+P` → **Wokwi: Request a Free License**)
- A [Blynk](https://blynk.cloud) account
- [Node.js](https://nodejs.org/) (LTS) for Node-RED

### 1. Set up Blynk

1. Create a template named **Smart Automation Hub** (Hardware: ESP32, Connection: WiFi).
2. Create these datastreams (Virtual Pins):

| Name | Pin | Type | Min | Max |
|---|---|---|---|---|
| Temperature | V0 | Double | -40 | 80 |
| Humidity | V1 | Double | 0 | 100 |
| Light Level | V2 | Integer | 0 | 4095 |
| Motion Status | V3 | Integer | 0 | 1 |
| Distance | V4 | Double | 0 | 400 |
| Fan Status | V5 | Integer | 0 | 1 |
| Light Status | V6 | Integer | 0 | 1 |

3. Create two events. In both, remove the condition row (the ESP32 triggers the events from code), enable notifications and set the limit to 1 notification per 1 minute:

| Code | Name | Type |
|---|---|---|
| `motion_alert` | Motion Detected | Info |
| `object_close` | Object Too Close | Warning |

   The event codes must match the code in `main.cpp` exactly.

4. Add dashboard widgets: gauges (V0, V1, V2, V4), LED widgets (V3, V5, V6) and charts.
5. Save the template, create a device from it, and copy the **Template ID**, **Template Name** and **Auth Token**.

### 2. Configure the project

Open `src/main.cpp` and paste your Blynk values:

```cpp
#define BLYNK_TEMPLATE_ID   "PASTE_YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "PASTE_YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN    "PASTE_YOUR_AUTH_TOKEN"
```

> **Do not commit your real Auth Token.** Keep the placeholders in the repository.

The MQTT settings are in the same file:

```cpp
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* topicPrefix = "YOUR_UNIQUE_PREFIX/automation/";
```

Replace `YOUR_UNIQUE_PREFIX` with your own unique value (for example a random string). The broker is public, so do not use your name or any personal detail in it. Use the same prefix in Node-RED.

### 3. Build and run

1. Click the PlatformIO **Build** button and wait for `SUCCESS`.
2. Open `diagram.json`, press `Ctrl+Shift+P` and run **Wokwi: Start Simulator**.
3. The Serial monitor should show the Blynk connection, `Connecting to MQTT...connected`, and a line every 2 seconds:

```
Temp: 30.50 | Humidity: 23.50 | Light: 166 | Motion: 0 | Distance: 338.01 | Fan: 1 | LightRelay: 0
```

### 4. Check the MQTT data (optional)

Open the [HiveMQ Websockets client](https://www.hivemq.com/demos/websocket-client/), use host `broker.hivemq.com`, port `8884` with SSL, connect, and subscribe to `YOUR_UNIQUE_PREFIX/automation/#`. Messages arrive every 2 seconds while the simulator is running.

### 5. Set up Node-RED

1. Install and start Node-RED: `npm install -g --unsafe-perm node-red`, then `node-red`.
2. Open `http://localhost:1880`.
3. Menu → **Manage Palette → Install** → install `node-red-dashboard`.
4. Build the flow shown below (one `mqtt in` node per topic, wired to a dashboard widget). Broker: `broker.hivemq.com`, port `1883`.
5. Click **Deploy** and open `http://localhost:1880/ui` (tab **Smart Hub**).

## MQTT topics and Node-RED flow

| Topic | Content | Dashboard widget | Range |
|---|---|---|---|
| `YOUR_UNIQUE_PREFIX/automation/temperature` | Temperature (°C) | Gauge "Temperature" | -40 to 80 |
| `YOUR_UNIQUE_PREFIX/automation/humidity` | Humidity (%) | Gauge "Humidity" | 0 to 100 |
| `YOUR_UNIQUE_PREFIX/automation/light` | LDR reading | Gauge "Light Level" | 0 to 4095 |
| `YOUR_UNIQUE_PREFIX/automation/distance` | Distance (cm) | Gauge "Distance" | 0 to 400 |
| `YOUR_UNIQUE_PREFIX/automation/motion` | 0 or 1 | Text "Motion (0/1)" | 0 or 1 |
| `YOUR_UNIQUE_PREFIX/automation/fan` | 0 or 1 | Text "Fan (0/1)" | 0 or 1 |
| `YOUR_UNIQUE_PREFIX/automation/lightstatus` | 0 or 1 | Text "Light (0/1)" | 0 or 1 |

## Automation logic

| Output | Rule |
|---|---|
| Fan (GPIO 26) | ON when temperature > 30.0 °C |
| Light (GPIO 25) | ON when LDR reading > 3000 **and** motion is detected |
| `motion_alert` | Sent when the PIR value changes from 0 to 1 |
| `object_close` | Sent when the distance is below 10 cm (re-armed when it is 10 cm or more) |

In Wokwi the LDR reading is **higher when it is darker**. The PIR output is high for only a few seconds and the code reads every 2 seconds, so trigger the motion again if `Motion: 1` does not appear.

## Testing

| Test | Action | Expected result |
|---|---|---|
| Fan | Set the DHT22 temperature above 30 °C | Blue LED ON, `Fan: 1` |
| Light | Make the LDR dark and trigger the PIR | Yellow LED ON, `LightRelay: 1` |
| Motion alert | Trigger the PIR | Blynk notification "Motion Detected" |
| Object close | Set the HC-SR04 distance below 10 cm | Blynk notification "Object Too Close" |
| Node-RED | Open `localhost:1880/ui` | Gauges and texts update |
| Blynk | Open the Blynk web dashboard | Gauges, LEDs and charts update |

## Troubleshooting

| Problem | What to check |
|---|---|
| Blynk does not connect | Re-copy the Template ID, Template Name and Auth Token; no extra spaces inside the quotes |
| `Event 'motion_alert' not found in device template` | Event codes in the Blynk template must be exactly `motion_alert` and `object_close`; save the template and make sure the device uses it |
| MQTT `failed, rc=-2` | The broker was not reachable; let WiFi connect first and check the internet and port 1883 |
| HiveMQ web client `Socket error` | Host `broker.hivemq.com`, port `8884` with SSL, or port `8000` without SSL; disable VPN or ad-blocker |
| No messages in the client | The Wokwi simulator must be running; check the topic `YOUR_UNIQUE_PREFIX/automation/#` |
| Node-RED gauge is empty | Topic spelling must match exactly, click Deploy, and the `mqtt in` node must show "connected" |
| Yellow LED never turns ON | Needs LDR reading above 3000 **and** `Motion: 1` at the same time |
| Wire not connected in Wokwi | The LDR pin is `AO`; drag overlapping parts apart |

## Notes

- The MQTT broker is public: do not publish private data, and use a unique topic prefix that contains no personal information (no name, email or location).
- Never commit your Blynk Auth Token or any other credentials.
- FlowFuse (browser-based Node-RED) uses Dashboard 2.0, so its dashboard nodes differ from `node-red-dashboard`.
