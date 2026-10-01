IoT Weather Dashboard (ESP32 + DHT22 + Blynk)

An IoT weather monitoring system built with an ESP32 microcontroller and a DHT22 sensor. Temperature and humidity are read in real time and displayed on a live Blynk IoT dashboard with gauges and a history chart. The project is developed and simulated entirely in the Wokwi simulator, running inside VS Code with PlatformIO, so no physical hardware is required.

Live Simulation

Wokwi project link: PASTE_YOUR_WOKWI_PROJECT_LINK_HERE

How It Works

DHT22 sensor → ESP32 → WiFi → Blynk Cloud → Mobile app / Web dashboard

The DHT22 sensor measures temperature and humidity.
The ESP32 reads these values every 2 seconds.
The values are sent to the Blynk Cloud over WiFi.
The Blynk dashboard displays them live on gauges and logs their history on a chart.
Components Used
Component	Purpose
ESP32	Main controller with built-in WiFi
DHT22	Measures temperature and humidity
Blynk IoT	Mobile app and cloud dashboard
Wokwi	Online simulator (replaces real hardware)
PlatformIO (VS Code)	Development environment used to build and run the simulation
Circuit Connections
DHT22 Pin	Connect To
VCC	ESP32 3V3
SDA	ESP32 GPIO 15
GND	ESP32 GND

Circuit image: image ki place ha

Blynk Setup
Create a template named IoT Weather Station Dashboard (Hardware: ESP32, Connection: WiFi).
Add two datastreams:
Temperature — Virtual Pin V0, type Double
Humidity — Virtual Pin V1, type Double
Add a Gauge widget for each datastream, and a Chart widget linked to both for historical logging.
Create a device from the template and copy the Template ID, Template Name, and Auth Token.
How to Run
Open the project folder in VS Code with the PlatformIO and Wokwi Simulator extensions installed.
Open platformio.ini and confirm the required libraries are listed (Blynk, DHT sensor library).
Open src/main.cpp and replace the three placeholder values with your own Blynk details.
Build the project (PlatformIO: Build).
Run the simulation (Command Palette → Wokwi: Start Simulator).
Watch the Serial Monitor for live temperature and humidity readings, and check the Blynk dashboard for the same values updating in real time.

Working screenshot: image ki place ha

Code Overview
Part	What It Does
dht.readTemperature() / dht.readHumidity()	Reads the current sensor values
isnan() check	Skips the update if a sensor reading fails
Blynk.virtualWrite(V0, ...) / Blynk.virtualWrite(V1, ...)	Sends temperature and humidity to the Blynk dashboard
timer.setInterval(2000L, sendWeatherData)	Repeats the read-and-send cycle every 2 seconds
Data Logging

Historical data is logged using Blynk's built-in Chart widget, which stores a time-based record of both temperature and humidity readings directly on the Blynk Cloud.

Important Note

Never upload your real Blynk Auth Token to a public repository. This project uses placeholders. Add your own token only on your computer or in Wokwi.

Tools and Technologies

ESP32, Arduino C/C++, Blynk IoT, Wokwi Simulator, PlatformIO, VS Code

Author

Danish GitHub: Danish3431-code
