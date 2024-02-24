#include <Arduino.h>

#define LED_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int delayTime = random(10, 50); // Random delay between 50ms and 500ms
  digitalWrite(LED_PIN, HIGH);
  delay(delayTime);
  digitalWrite(LED_PIN, LOW);
  delay(delayTime);
}