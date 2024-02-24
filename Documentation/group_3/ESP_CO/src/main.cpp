#include <Arduino.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFi.h>

// #include <WiFiClient.h>
// #include <DHT.h>
// // https://github.com/LilyGO/LILYGO-T-Energy/tree/master


// // OTA update with elegantOTA
// #include <AsyncTCP.h>
// #include <WebServer.h>
// #include <ElegantOTA.h>


#define AO_PIN 33  // ESP32's pin GPIO36 connected to AO pin of the MQ2 sensor

void setup() {
  // initialize serial communication
  Serial.begin(115200);
  Serial.println("Warming up the MQ2 sensor");
  delay(20000);  // wait for the MQ2 to warm up
}

void loop() {
  int gasValue = analogRead(AO_PIN);
  Serial.print("MQ2 sensor AO value: ");
  Serial.println(gasValue);
}

























