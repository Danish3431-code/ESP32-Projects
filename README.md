# Mobile Controlled Light (ESP32 + Blynk + Wokwi)

A simple IoT project where an LED (light) is turned ON and OFF from a mobile phone using an ESP32 and the Blynk app. The whole project runs in the **Wokwi simulator**, so no physical hardware is needed.

## Live Simulation

Wokwi project link: `PASTE_YOUR_WOKWI_PROJECT_LINK_HERE`

## How It Works

Phone (Blynk app) → Internet → Blynk Cloud → ESP32 → LED

1. You press the switch in the Blynk app.
2. The command travels through the internet to the Blynk Cloud.
3. The Blynk Cloud forwards it to the ESP32, which is connected to WiFi.
4. The ESP32 turns the LED ON or OFF.

## Components Used

| Component | Purpose |
|---|---|
| ESP32 | Main controller with built-in WiFi |
| LED | Acts as the light |
| 220 Ω Resistor | Protects the LED from too much current |
| Blynk IoT | Mobile app and cloud for remote control |
| Wokwi | Online simulator (replaces real hardware) |

## Circuit Connections

| From | To |
|---|---|
| ESP32 GPIO 26 | Resistor |
| Resistor | LED anode (long leg) |
| LED cathode (short leg) | ESP32 GND |

**Circuit image:** image ki place ha

## Blynk Setup

1. Create a template named `Mobile Light` (Hardware: ESP32, Connection: WiFi).
2. Add a datastream: name `Light`, Virtual Pin `V0`, type Integer, min 0, max 1.
3. Add a Switch widget linked to `V0`.
4. Create a device from the template and copy the Template ID, Template Name, and Auth Token.

## How to Run

1. Open the project in Wokwi.
2. Add the **Blynk** library from the Library Manager.
3. Open `sketch.ino` and replace the three placeholder values with your own Blynk details.
4. Click **Start Simulation**.
5. Toggle the switch in the Blynk app or web dashboard. The LED turns ON and OFF.

**Working screenshot:** image ki place ha

## Code Overview

| Part | What It Does |
|---|---|
| `BLYNK_WRITE(V0)` | Runs automatically when the app switch changes and sets the LED ON or OFF |
| `setup()` | Runs once: sets the LED pin as output and connects to WiFi and Blynk |
| `loop()` | Runs forever: keeps the Blynk connection alive |

## Important Note

Never upload your real Blynk Auth Token to a public repository. This project uses placeholders. Add your own token only on your computer or in Wokwi.

## Tools and Technologies

ESP32, Arduino C/C++, Blynk IoT, Wokwi Simulator

## Author

Danish
GitHub: [Danish3431-code](https://github.com/Danish3431-code)
