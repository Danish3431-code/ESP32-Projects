#define BLYNK_TEMPLATE_ID "PLACE YOUR TempLATE_ID"
#define BLYNK_TEMPLATE_NAME "PLACE YOUR TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "PLACE YOUR TOKENS"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define DHT_PIN 15
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;

// Reads the sensor and sends values to Blynk
void sendWeatherData()
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // isnan() checks if the reading failed (sensor not ready yet)
    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
}

void setup()
{
    Serial.begin(115200);
    dht.begin();
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // Read and send data every 2 seconds
    timer.setInterval(2000L, sendWeatherData);
}

void loop()
{
    Blynk.run();
    timer.run();
}
