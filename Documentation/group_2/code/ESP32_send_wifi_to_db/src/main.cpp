#include <Arduino.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include <WiFiClient.h>
#include <DHT.h>



#include "utils.h"




HTTPClient http;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

#include "DHTesp.h"
DHTesp dht;


int r_led = 18;
int g_led = 34;
int b_led = 33;


// ------------------setup function-----------------
void setup() {
  Serial.begin(115200);

  delay(1000);     

  WiFi.mode(WIFI_STA);                                                                                     
  delay(1000);     
  WiFi.begin(ssid, password);
  delay(1000);     
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }

  Serial.println("Connected to the WiFi network");


  dht.setup(TEMPERATUREANDHUMIDITY_PIN, DHTesp::DHT22);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(5, OUTPUT); // Set the LED pin as an output
  // randomSeed(analogRead(0)); // Seed the ra


}

float getTemperature(bool condition) {
  if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getHumidity(bool condition) {
  if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getCo(bool condition) {
if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}
float getC2h5oh(bool condition) {
if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getH2(bool condition) {
if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getNo2(bool condition) {
if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getNh3(bool condition) {
if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getSoilMoisture(bool condition) {
if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getDistance(bool condition) {
if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getAirPressure(bool condition) {
if (condition) {
    return 100;
  } else {
    return -1.0;
  }
}

float getAirQuality(bool condition) {
  if (condition) {
    return 100.0;
  } else {
    return -1.0;
  }
}

float getWifi(bool condition) {
  if (condition) {
    return WiFi.scanNetworks();
  } else {
    return -1.0;
  }
}

const float VOLTAGE_REFERENCE = 3.3;  // Reference voltage of the ADC

float getVoltage(bool condition) {
  if (condition) {
    int analogReading = analogRead(BATTERYPIN);
    float voltage = (analogReading / 1023.0) * VOLTAGE_REFERENCE;
    return voltage;
  } else {
    return -1.0;
  }
}



// generate json file


String generateJSON(String field, float data = -1.0, bool end = false) {
  if (data != -1.0) {
    String result = "\"" + field + "\": " + String(data, 3);
    
    if (!end) {
      result += ",";
    }

    return result;
  } else {
    return "";
  }
}



void sendData(int voltage, float temperature = -1.0, float humidity = -1.0, float co = -1.0, float c2h5oh = -1.0, float h2 = -1.0, float no2 = -1.0, float nh3 = -1.0, float soil_moisture = -1.0, float distance = -1.0, float air_pressure = -1.0, float air_quality = -1.0, float wifi = -1.0) {
  HTTPClient http;

  String url = "https://api.archigrad.io/?db=group_2&table=sensor_data";

  http.begin(url);
  http.addHeader("Content-Type", "application/json");



  String json = R"({
      "mac_address": ")" + String(WiFi.macAddress()) + R"(",
      "voltage": )" + String(voltage) + R"(
    )";

  if (temperature != -1.0) {
    json += R"(,"temperature":)" + String(temperature,2);
  }

  if (humidity != -1.0) {
    json += R"(,"humidity":)" + String(humidity,2);
  }

  if (co != -1.0) {
    json += R"(,"co":)" + String(co,2);
  }


  if (c2h5oh != -1.0) {
    json += R"(,"c2h5oh":)" + String(c2h5oh,2);
  }


  if (h2 != -1.0) {
    json += R"(,"h2":)" + String(h2,2);
  }


  if (no2 != -1.0) {
    json += R"(,"no2":)" + String(no2,2);
  }


  if (nh3 != -1.0) {
    json += R"(,"nh3":)" + String(nh3,2);
  }


  if (soil_moisture != -1.0) {
    json += R"(,"soil_moisture":)" + String(soil_moisture,2);
  }


  if (distance != -1.0) {
    json += R"(,"distance":)" + String(distance,2);
  }


  if (air_pressure != -1.0) {
    json += R"(,"air_pressure":)" + String(air_pressure,2);
  }


  if (air_quality != -1.0) {
    json += R"(,"air_quality":)" + String(air_quality,2);
  }


  if (wifi != -1.0) {
    json += R"(,"wifi":)" + String(wifi,2);
  }


  json += R"(
  })";

  int httpResponseCode = http.POST(json);

  Serial.print("HTTP Response Code: ");
  Serial.println(httpResponseCode);

  Serial.print("Response: ");
  Serial.println(http.getString());

  http.end();
}


void ledColor(int rValue, int gValue, int bValue) {
    analogWrite(r_led, rValue);
    analogWrite(g_led, gValue);
    analogWrite(b_led, bValue);
}


// ------------------------loop function-------------
void loop() {


  // int interval = random(500, 3000); // Generate a random interval between 500 and 3000 milliseconds
  digitalWrite(5, HIGH); // Turn on the LED
  delay(100); // Wait for the random interval
  digitalWrite(5, LOW); // Turn off the LED
  delay(100); // Wait for a short time before the next blink


  TempAndHumidity measurement = dht.getTempAndHumidity();
  float temperature = measurement.temperature;
  float humidity = measurement.humidity;
  
  int sumSound = 0; 
  int n = 0;
  for (int i = 0; i < 500; i++) {
    n +=1;
    sumSound += analogRead(32);
    Serial.println(sumSound);
    ledColor(random(i),0,random(i)); 
    delay(10);
  }
// 
  // delay(2000);
  ledColor(0,0,0);
  int voltage = analogRead(BATTERYPIN);
  sendData(

  voltage,     // voltage
  -1,         // temperature
  -1,         // humidity
  -1,         //co
  -1,         //c2h5oh
  -1,         //h2
  -1,         // no2
  -1,         // nh3
  -1,         // soil_moisture
  -1,         // distance ut now sound
  -1,         // air_pressure
  -1,         // air_quality
   WiFi.scanNetworks()          // wifi

);
  // delay(1000);
   // red
  Serial.println(sumSound); //sound

  // Serial.println("ip address is: " + WiFi.localIP().toString() + "  mac address is: " + WiFi.macAddress());

}

