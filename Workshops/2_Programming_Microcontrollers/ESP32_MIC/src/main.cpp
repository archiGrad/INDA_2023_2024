#include <Arduino.h>

#define LED_PIN 5

int r_led = 12;
int g_led = 14;
int b_led = 27;



void setup() {
  Serial.begin(115200);
  pinMode(r_led, OUTPUT);
  pinMode(g_led, OUTPUT);
  pinMode(b_led, OUTPUT);
}

void ledColor(int rValue, int gValue, int bValue) {
    analogWrite(r_led, rValue);
    analogWrite(g_led, gValue);
    analogWrite(b_led, bValue);
}


void loop() {
  ledColor(analogRead(25)/2,0,analogRead(25)/2); // red
  
  Serial.println(analogRead(25));
  delay(100);
}

