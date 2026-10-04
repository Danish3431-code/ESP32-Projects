#define BLYNK_TEMPLATE_ID "Enter Your Template ID"
#define BLYNK_TEMPLATE_NAME "Enter Your Project Name"
#define BLYNK_AUTH_TOKEN "Enter Your Auth Token"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define DHT_PIN 15
#define DHT_TYPE DHT22
#define LDR_PIN 34
#define PIR_PIN 27
#define TRIG_PIN 18
#define ECHO_PIN 19
#define DISTANCE_ALERT_THRESHOLD 10.0 // cm — isse kam ho to alert

DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;

bool lastMotionState = false;
bool distanceAlertSent = false;

float readDistanceCM()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // timeout 30ms
    float distance = duration * 0.034 / 2;          // sound speed formula

    return distance;
}

void readAllSensors()
{
    // ---- Temperature & Humidity ----
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity))
    {
        Blynk.virtualWrite(V0, temperature);
        Blynk.virtualWrite(V1, humidity);
    }

    // ---- Light Level ----
    int lightLevel = analogRead(LDR_PIN);
    Blynk.virtualWrite(V2, lightLevel);

    // ---- Motion Detection ----
    bool motionDetected = digitalRead(PIR_PIN);
    Blynk.virtualWrite(V3, motionDetected ? 1 : 0);

    // Sirf tab alert bhejo jab motion "newly" detect ho (continuously spam na ho)
    if (motionDetected && !lastMotionState)
    {
        Blynk.logEvent("motion_alert", "Motion detected near the sensor!");
        Serial.println("ALERT: Motion detected!");
    }
    lastMotionState = motionDetected;

    // ---- Distance (Ultrasonic) ----
    float distance = readDistanceCM();
    Blynk.virtualWrite(V4, distance);

    if (distance > 0 && distance < DISTANCE_ALERT_THRESHOLD && !distanceAlertSent)
    {
        Blynk.logEvent("object_close", "Object detected too close!");
        Serial.println("ALERT: Object too close!");
        distanceAlertSent = true;
    }
    else if (distance >= DISTANCE_ALERT_THRESHOLD)
    {
        distanceAlertSent = false; // reset taake agli baar dobara alert ja sake
    }

    // ---- Serial Monitor Debug ----
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" C | Humidity: ");
    Serial.print(humidity);
    Serial.print(" % | Light: ");
    Serial.print(lightLevel);
    Serial.print(" | Motion: ");
    Serial.print(motionDetected);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}

void setup()
{
    Serial.begin(115200);
    dht.begin();

    pinMode(PIR_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    timer.setInterval(2000L, readAllSensors);
}

void loop()
{
    Blynk.run();
    timer.run();
}
