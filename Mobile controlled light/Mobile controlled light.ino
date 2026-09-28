// Mobile Controlled Light - ESP32 + Blynk (Wokwi simulation)
// NOTE: Replace the placeholders below with your own Blynk details.
// Never upload your real Auth Token to a public GitHub repository.

#define BLYNK_TEMPLATE_ID   "PASTE_YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "PASTE_YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN    "PASTE_YOUR_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Wokwi-GUEST";   // Wokwi's virtual WiFi
char pass[] = "";              // no password

#define LED_PIN 26

// Runs automatically when the Blynk switch (V0) changes
BLYNK_WRITE(V0) {
  int value = param.asInt();   // 1 = ON, 0 = OFF
  digitalWrite(LED_PIN, value);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
}
