#include <Arduino.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include <WiFiClient.h>
#include <DHT.h>
// https://github.com/LilyGO/LILYGO-T-Energy/tree/master


// OTA update with elegantOTA
// #include <AsyncTCP.h>
// #include <WebServer.h>
// #include <ElegantOTA.h>

#include "utils.h"




HTTPClient http;

// start webserver on port 80
// WebServer server(80);

// const char* ssid = "TP-Link_6B60";
// const char* password = "Ikegcasen72.";

// homenetwork
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

#include "DHTesp.h"
DHTesp dht;



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
  // Serial.println(WiFi.localIP());
  // Serial.println(WiFi.macAddress());

  dht.setup(TEMPERATUREANDHUMIDITY_PIN, DHTesp::DHT22);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);


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




// void sendData(String mac_address) {
//     // http.begin("https://api.archigrad.io/?db=" DATABASE "&table=sensor_data");
//     http.begin("https://api.archigrad.io/?db=group_1&table=sensor_data");
//     // delay(100);
//     http.addHeader("Content-Type", "application/json");
//     // delay(100);

//     String json = 
//         "{"
//         "\"mac_address\": \"" + mac_address + "\"," + // macaddress

//         generateJSON("temperature", getTemperature(true)) + // temperature
//         generateJSON("humidity",getHumidity(false)) + // humidity
//         generateJSON("co", getCo(false)) + // co
//         generateJSON("c2h5oh", getC2h5oh(false)) + // cc2h5oh
//         generateJSON("h2", getH2(false)) + // h2
//         generateJSON("no2", getNo2(false)) + // no2
//         generateJSON("nh3", getNh3(false)) + // nh3
//         generateJSON("soil_moisture", getSoilMoisture(false)) + // soil_moisture
//         generateJSON("distance", getDistance(false)) + // distance
//         generateJSON("air_pressure", getAirPressure(false)) + // air_pressure
//         generateJSON("air_quality", getAirQuality(true)) + // air_quality
//         generateJSON("wifi", getWifi(true)) + // wifi
//         generateJSON("voltage", getVoltage(true), true) + // voltage (second argument means this is the end of the json string)
      
//         "}";

//     int httpResponseCode = http.POST(json);
//     // delay(100);
//     // Serial.println(httpResponseCode);

//     // print when succesful response
//     if (httpResponseCode == 200){
//       digitalWrite(LED_PIN, HIGH); 
//       delay(100);
//       digitalWrite(LED_PIN, LOW);
//     }

//     // Serial.println(json);
//     http.end();
// }
// void sendData(int voltage) {
void sendData(int voltage, float temperature = -1.0, float humidity = -1.0) {
  HTTPClient http;

  http.begin("https://api.archigrad.io/?db=group_3&table=sensor_data");
  http.addHeader("Content-Type", "application/json");



  String json = R"({
      "mac_address": ")" + WiFi.macAddress() + R"(",
      "voltage": )" + String(voltage) + R"(
    )";

  if (temperature != -1.0) {
    json += R"(,"temperature":)" + String(temperature,2);
  }

  if (humidity != -1.0) {
    json += R"(,"humidity":)" + String(humidity,2);
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



// ------------------------loop function-------------
void loop() {
  TempAndHumidity measurement = dht.getTempAndHumidity();
  float temperature = measurement.temperature;
  float humidity = measurement.humidity;
  

  Serial.println(temperature);
  Serial.println(humidity);
  // int soundVoltage = analogRead(33);
  delay(2000);
  // sendData(WiFi.macAddress());
  int voltage = analogRead(BATTERYPIN);
  sendData(
  voltage,          // voltage
  temperature,        // temperature
  humidity         // humidity
);
  // delay(1000);

  Serial.println("----------------- ");
  Serial.println("ip address is: " + WiFi.localIP().toString() + "  mac address is: " + WiFi.macAddress());

}








// houdini

// import requests
// import json
// import datetime

// # node = hou.pwd()
// # geo = node.geometry()


// DB = "group_1"
// TABLE = "sensor_data"
// SLIDER = hou.evalParm("slider")
// URL = f"https://api.archigrad.io/?db={DB}&table={TABLE}"
// TIMEZONE = 1


// # functions
// def slider_control(url, field):
//     # normalive values between 0 and 1
//     response = requests.get(url + "&minmax=" + field )
//     json_response = response.json()
//     min_id = int(json_response["min"])
//     max_id = int(json_response["max"])
//     value =( SLIDER * (max_id-min_id) ) + min_id
//     return round(value)
    

// def get_request(url, arg): 
//     time_interval = 2 #amount of time between every measurment in s. c++ -> delay(2000);
//     treshold = round(time_interval/2)
//     response = requests.get(url + "&time_utc=" + str(arg-treshold) + "-" + str(arg+treshold) ) 
//     return json.dumps(response.json(), indent=2)
//     # return response.json()

// def unixtime_convert(unix_timestamp, timezone):

//     timestamp_utc = datetime.datetime.utcfromtimestamp(unix_timestamp)
//     # Add 1 hour (3600 seconds) to the timestamp
//     timestamp_with_hour_added = timestamp_utc + datetime.timedelta(seconds=(TIMEZONE * 3600))
//     readable_time = timestamp_with_hour_added.strftime('%Y-%m-%d %H:%M:%S')
//     return readable_time


// print(unixtime_convert(slider_control(URL, "time_utc"), TIMEZONE))
    
// # print(get_request(URL, slider_control(URL, "time_utc") ))



// data = json.loads(get_request(URL, slider_control(URL, "time_utc") ))

// # print(len(data))
// # print(data)
// # print(type(get_request(URL, slider_control(URL, "time_utc") )))


// for item in data:
//     for key, value in item.items():
//         print(f"{key}: {value}")
//     print("-" * 30)






// new py
// post sensor data
// # curl

// # curl -X POST "https://api.archigrad.io/?db=group_1&table=sensor_data" \
// #   -H "Content-Type: application/json" \
// #   -d '{
// #     "mac_address": "zzz",
// #     "distance": 15.0,
// #     "temperature": 2000,
// #     "air_pressure": 1015.0,
// #     "air_quality": 80.0,
// #     "wifi": 90.0,
// #     "voltage": 1,
// #     "time_utc": 1639827540,
// #     "wifi": 10
// #   }'


// import requests

// url = "https://api.archigrad.io/?db=group_1&table=sensor_data"

// payload = { 
//     "mac_address": "0C:B8:15:1A:76:04",
//     "sensor_id": 1000,
//     "humidity": 60.0,
//     "co": 0.3,
//     "c2h5oh": 10.5,
//     "h2": 2.0,
//     "no2": 0.02,
//     "nh3": 0.01,
//     "soil_moisture": 30.0,
//     "distance": 15.0,
//     "air_pressure": 1015.0,
//     "air_quality": 80.0,
//     "wifi": 90.0,
//     "voltage": 3.7
    
// }

// headers = {
//     "Content-Type": "application/json"
// }





// response = requests.post(url, json=payload, headers=headers)

// # Print the response
// print(response.text)



// // py put sensor data
// import requests

// db = "group_1"
// table = "sensor_data"
// id_to_update = 73

// url = f"https://api.archigrad.io/?db={db}&table={table}&id={id_to_update}"

// payload = { 
//     "mac_address": "zzz",
//     "sensor_id": 1000,
//     "humidity": 60.0,
//     "co": 0.3,
//     "c2h5oh": 10.5,
//     "h2": 2.0,
//     "no2": 0.02,
//     "nh3": 0.01,
//     "soil_moisture": 30.0,
//     "distance": 15.0,
//     "air_pressure": 1015.0,
//     "air_quality": 80.0,
//     "wifi": 90.0,
//     "voltage": 3.7,
//     "time_utc": 1639827540,
//     "id": id_to_update
// }

// headers = {
//     "Content-Type": "application/json"
// }


// # Make the PUT request
// response = requests.put(url, json=payload, headers=headers)

// # Print the result as JSON
// print(response.json())


// // py delete
// import requests

// db = "group_1"
// table = "sensor_data"
// id_to_delete = "1-6000"  

// url = f"https://api.archigrad.io/?db={db}&table={table}&id={id_to_delete}"

// # Make the DELETE request
// response = requests.delete(url)

// # Print the result as JSON
// print(response.json())

























// crash course soldering
// https://www.youtube.com/watch?v=6rmErwU5E-k

// design our own custom esp32 board
// https://www.youtube.com/watch?v=S_p0YV-JlfU

// a good example of sending humidity and temperature datat to a webserver over wifi
// https://arduino.stackexchange.com/questions/92477/wificlient-cpp517-flush-fail-on-fd-48-errno-11-no-more-processes-what-i
// code can be seen below
// #include <Arduino.h>
// #include <DHT.h>
// #include <Adafruit_Sensor.h>
// #include <WiFi.h>
// #include <HTTPClient.h>
// #include <Wire.h>

// // FOR T sensor
// #define TsensorPin 4
// #define DHTTYPE DHT22
// DHT dht(TsensorPin, DHTTYPE);
// String sensorName = "DHT22";
// String sensorLocation = "Casa1";
// float h;
// float t;
// float f;
// float hif;
// float hic;

// // for wifi connection
// const char* ssid     = "Captain Spiko";
// const char* password = "123456789d";

// // server data
// const char* serverName = "http://192.168.50.158/esp_web/post_esp.php";
// String apiKeyValue = "123456789";

// // for awake and sleep
// #define uS_TO_S_FACTOR 1000000  
// #define TIME_TO_SLEEP  300  
// RTC_DATA_ATTR int bootCount = 0;

// void print_wakeup_reason(){
//   esp_sleep_wakeup_cause_t wakeup_reason;

//   wakeup_reason = esp_sleep_get_wakeup_cause();

//   switch(wakeup_reason)
//   {
//     case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
//     case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
//     case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
//     case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
//     case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
//     default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
//   }
// }

// void readTemperature(){
  
//   h = dht.readHumidity();
//   t = dht.readTemperature();
//   f = dht.readTemperature(true); // in F
  
//   // Compute heat index in Fahrenheit (the default)
//   hif = dht.computeHeatIndex(f, h);
//   // Compute heat index in Celsius (isFahreheit = false)
//   hic = dht.computeHeatIndex(t, h, false);

//   Serial.print(F("Humidity: "));
//   Serial.print(h);
//   Serial.print(F("%  Temperature: "));
//   Serial.print(t);
//   Serial.print(F("°C "));
//   Serial.print(f);
//   Serial.print(F("°F  Heat index: "));
//   Serial.print(hic);
//   Serial.print(F("°C "));
//   Serial.print(hif);
//   Serial.println(F("°F"));
// }

// void activateSleep(){
//   Serial.println("Going to sleep now");
//   delay(1000);
//   Serial.flush(); 
//   esp_deep_sleep_start();
// }

// bool setupWifi(){
//   WiFi.begin(ssid, password);
//   Serial.println("Connecting");
//   while(WiFi.status() != WL_CONNECTED) { 
//     delay(500);
//     Serial.print(".");
//   } 
//   Serial.println("");
//   Serial.print("Connected to WiFi network with IP Address: ");
//   Serial.println(WiFi.localIP());
 
//   if (WiFi.status() == WL_CONNECTED){
//     return true;
//   } else {
//     return false;
//   }



// }

// void postData(){
//   if(WiFi.status()== WL_CONNECTED){
//     WiFiClient client;
//     HTTPClient http;
//     http.begin(client, serverName);
//     http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//     // Prepare your HTTP POST request data
//     String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName
//                           + "&location=" + sensorLocation + "&value1=" + String(h)
//                           + "&value2=" + String(t) + "&value3=" + String(f) + "";
//     Serial.print("httpRequestData: ");
//     Serial.println(httpRequestData);
//     int httpResponseCode = http.POST(httpRequestData);
//     if (httpResponseCode>0) {
//       Serial.print("HTTP Response code: ");
//       Serial.println(httpResponseCode);
//     }
//     else {
//       Serial.print("Error code: ");
//       Serial.println(httpResponseCode);
//     }
//     http.end();
//   }else {
//     Serial.println("WiFi Disconnected");
//   }
// }

// /// @brief  Set up
// void setup() {
  
//   Serial.begin(115200);
//   delay(1000);
//   ++bootCount; // tiene il conto dei restart
//   Serial.println("Boot number: " + String(bootCount));
//   print_wakeup_reason();
//   if (setupWifi() == true){
//     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);// set up next wake up after 30 sec
//     Serial.println(F("DHTxx test!"));
//     dht.begin();
//   };
  
  

// }

// /// @brief loop func
// void loop() {
//   readTemperature();
//   postData();
//   activateSleep();
// }
