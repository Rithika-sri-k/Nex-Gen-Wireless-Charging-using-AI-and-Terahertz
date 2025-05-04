#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT11
#define IRSENSOR 8

DHT dht(DHTPIN, DHTTYPE);

bool wasDetected = false;  // Tracks object state
bool batteryShown = false; // Tracks if 18% battery shown once

void setup() {
  Serial.begin(9600);
  pinMode(IRSENSOR, INPUT_PULLUP); // Use internal pull-up resistor
  dht.begin();
  Serial.println("Prototype Booted...");
}

void loop() {
  bool objectDetected = digitalRead(IRSENSOR) == LOW;

  float temperature = 0;
  int batteryLevel = 0;
  String aiDecision = "-";
  String chargingSpeed = "-";
  String status = "Phone not placed.";

  if (!objectDetected) {
    // Reset batteryShown flag when phone is removed
    batteryShown = false;
  }

  if (objectDetected) {
    status = "Charging...";
    temperature = dht.readTemperature();

    // Show 18% battery once
    if (!batteryShown) {
      batteryLevel = 18;
      aiDecision = "Boost speed (Low Batt)";
      chargingSpeed = "Fast";
      batteryShown = true;
    } else {
      batteryLevel = 55;  // Generic value for normal use
      if (temperature > 40) {
        aiDecision = "Reduce speed (Hot)";
        chargingSpeed = "Slow";
      } else {
        aiDecision = "Normal charging";
        chargingSpeed = "Normal";
      }
    }
  }

  // Display
  Serial.println("---- Prototype Status ----");
  Serial.print("Object: "); Serial.println(objectDetected ? "Phone Detected" : "No Object");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Battery Level: "); Serial.print(batteryLevel); Serial.println("%");
  Serial.print("Charging Speed: "); Serial.println(chargingSpeed);
  Serial.print("AI Decision: "); Serial.println(aiDecision);
  Serial.print("Status: "); Serial.println(status);
  Serial.println("---------------------------\n");

  delay(3000);
}