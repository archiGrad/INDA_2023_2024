#include <Arduino.h>

#define LED_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

digitalWrite(LED_PIN, HIGH);
  delay(1000); 
  digitalWrite(LED_PIN, LOW);
  delay(1000); 
}

