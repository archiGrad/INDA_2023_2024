#include <Arduino.h>
#include <WiFi.h>

#define LED_PIN 5

const char* ssid = "TP-Link_6655";
const char* password = "60767491";


void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);                                                                                 
  WiFi.begin(ssid, password);
  
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      digitalWrite(LED_PIN, HIGH);
      delay(50); 
      digitalWrite(LED_PIN, LOW);
      delay(50); 
  }

  Serial.println("Connected to the WiFi network");
  digitalWrite(LED_PIN, HIGH);
}

void loop() {
  
  delay(5000); 
  Serial.println("the local IP is:  ");
  Serial.println(WiFi.localIP());
  Serial.println("the MAC adress is:  ");
  Serial.println(WiFi.macAddress());
}

