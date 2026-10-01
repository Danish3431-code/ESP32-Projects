#define BLYNK_TEMPLATE_ID "PLACE YOUR TEMPLATE_ID "
#define BLYNK_TEMPLATE_NAME "PLACE YOUR TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "PLACE YOUR AUTH_TOKENS"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

#define DHT_PIN 15
#define DHT_TYPE DHT22
#define HEATER_PIN 25

DHT dht(DHT_PIN, DHT_TYPE);
BlynkTimer timer;

// ---------- PID Settings ----------
double setpoint = 30.0; // Target temperature (app se bhi change ho sakta hai)
double Kp = 8.0;        // Proportional gain
double Ki = 0.5;        // Integral gain
double Kd = 2.0;        // Derivative gain

double previousError = 0;
double integral = 0;

// App se setpoint change hone pe ye chalega
BLYNK_WRITE(V1)
{
  setpoint = param.asDouble();
}

double computePID(double currentTemp)
{
  double error = setpoint - currentTemp;

  integral += error;
  // Integral windup se bachne ke liye limit lagate hain
  if (integral > 100)
    integral = 100;
  if (integral < -100)
    integral = -100;

  double derivative = error - previousError;
  previousError = error;

  double output = (Kp * error) + (Ki * integral) + (Kd * derivative);

  // Output ko 0-255 (PWM range) mein limit karo
  if (output > 255)
    output = 255;
  if (output < 0)
    output = 0;

  return output;
}

void controlLoop()
{
  float currentTemp = dht.readTemperature();

  if (isnan(currentTemp))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  double pidOutput = computePID(currentTemp);

  // Heater ko PWM signal bhejo (0-255)
  analogWrite(HEATER_PIN, (int)pidOutput);

  // Blynk ko update bhejo
  Blynk.virtualWrite(V0, currentTemp);
  Blynk.virtualWrite(V2, (pidOutput / 255.0) * 100); // percentage mein

  Serial.print("Setpoint: ");
  Serial.print(setpoint);
  Serial.print(" | Current: ");
  Serial.print(currentTemp);
  Serial.print(" | PID Output: ");
  Serial.println(pidOutput);
}

void setup()
{
  Serial.begin(115200);
  dht.begin();
  pinMode(HEATER_PIN, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, controlLoop);
}

void loop()
{
  Blynk.run();
  timer.run();
}
