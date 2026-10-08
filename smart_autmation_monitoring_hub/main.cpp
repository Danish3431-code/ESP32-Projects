#define BLYNK_TEMPLATE_ID   "PASTE_YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "PASTE_YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN    "PASTE_YOUR_AUTH_TOKEN"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <PubSubClient.h>
char ssid[] = "Wokwi-GUEST";
char pass[] = "";
// ---------- MQTT Settings ---------
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* topicPrefix = "danish/automation/";
WiFiClient espClient;
PubSubClient mqttClient(espClient);
// ---------- Pins ---------
#define DHT_PIN      15
#define DHT_TYPE     DHT22
#define LDR_PIN      34
#define PIR_PIN      27
#define TRIG_PIN     18
#define ECHO_PIN     19
#define FAN_RELAY    26
#define LIGHT_RELAY  25
#define DISTANCE_ALERT_THRESHOLD 10.0
DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;
bool lastMotionState = false;
bool distanceAlertSent = false;
float readDistanceCM() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH, 30000);
    return duration * 0.034 / 2;
}
void connectMQTT() {
    while (!mqttClient.connected()) {
        Serial.print("Connecting to MQTT...");
        String clientId = "ESP32-Client-" + String(random(0xffff), HEX);
        if (mqttClient.connect(clientId.c_str())) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqttClient.state());
            Serial.println(" retrying in 2s");
            delay(2000);
        }
    }
}
void publishMQTT(const char* subtopic, float value) {
    char topic[60];
    char payload[20];
    snprintf(topic, sizeof(topic), "%s%s", topicPrefix, subtopic);
    snprintf(payload, sizeof(payload), "%.2f", value);
    mqttClient.publish(topic, payload);
}
void runAutomationAndPublish() {
    // ---- Read Sensors ---
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int lightLevel = analogRead(LDR_PIN);
    bool motionDetected = digitalRead(PIR_PIN);
    float distance = readDistanceCM();
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
    // ---- Automation Logic ---
Smart Automation & Monitoring Hub
    bool fanShouldBeOn = (temperature > 30.0);
    bool lightShouldBeOn = (lightLevel > 3000) && motionDetected;   // dark + motion
    digitalWrite(FAN_RELAY, fanShouldBeOn ? HIGH : LOW);
    digitalWrite(LIGHT_RELAY, lightShouldBeOn ? HIGH : LOW);
    // ---- Send to Blynk ---
    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);
    Blynk.virtualWrite(V2, lightLevel);
    Blynk.virtualWrite(V3, motionDetected ? 1 : 0);
    Blynk.virtualWrite(V4, distance);
    Blynk.virtualWrite(V5, fanShouldBeOn ? 1 : 0);
    Blynk.virtualWrite(V6, lightShouldBeOn ? 1 : 0);
    if (motionDetected && !lastMotionState) {
        Blynk.logEvent("motion_alert", "Motion detected near the sensor!");
    }
    lastMotionState = motionDetected;
    if (distance > 0 && distance < DISTANCE_ALERT_THRESHOLD && !distanceAlertSent) {
        Blynk.logEvent("object_close", "Object detected too close!");
        distanceAlertSent = true;
    } else if (distance >= DISTANCE_ALERT_THRESHOLD) {
        distanceAlertSent = false;
    }
    // ---- Publish to MQTT (Node-RED dashboard) ---
    if (!mqttClient.connected()) {
        connectMQTT();
    }
    publishMQTT("temperature", temperature);
    publishMQTT("humidity", humidity);
    publishMQTT("light", lightLevel);
    publishMQTT("motion", motionDetected ? 1 : 0);
    publishMQTT("distance", distance);
    publishMQTT("fan", fanShouldBeOn ? 1 : 0);
    publishMQTT("lightstatus", lightShouldBeOn ? 1 : 0);
    // ---- Debug ---
    Serial.print("Temp: "); Serial.print(temperature);
    Serial.print(" | Humidity: "); Serial.print(humidity);
    Serial.print(" | Light: "); Serial.print(lightLevel);
    Serial.print(" | Motion: "); Serial.print(motionDetected);
    Serial.print(" | Distance: "); Serial.print(distance);
    Serial.print(" | Fan: "); Serial.print(fanShouldBeOn);
    Serial.print(" | LightRelay: "); Serial.println(lightShouldBeOn);
}
void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(PIR_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(FAN_RELAY, OUTPUT);
    pinMode(LIGHT_RELAY, OUTPUT);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    mqttClient.setServer(mqttServer, mqttPort);
    connectMQTT();
    timer.setInterval(2000L, runAutomationAndPublish);
}
void loop() {
    Blynk.run();
    mqttClient.loop();
    timer.run();
}
