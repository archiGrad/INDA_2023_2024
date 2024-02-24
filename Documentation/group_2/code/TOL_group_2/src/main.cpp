

#include <Wire.h>
// include for screen
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// include for wifi and http requests
#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>


#include <images.h> //get images.h file,Tu here i store my img data.S

// OTA update with elegantOTA
#include <AsyncTCP.h>
#include <WebServer.h>
#include <ElegantOTA.h>


// define screen settings
#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ip: 192.168.1.96


// images to memory conversion here -> http://javl.github.io/image2cpp/
// images are references from images.h and images.cpp
const unsigned char *archiGradStartupAnim [] = {epd_bitmap_f0,	epd_bitmap_f1,	epd_bitmap_f3,	epd_bitmap_f4,	epd_bitmap_f5,	epd_bitmap_f6,	epd_bitmap_f7,	epd_bitmap_f8,	epd_bitmap_f9,  epd_bitmap_f10,	epd_bitmap_f11,	epd_bitmap_f12,	epd_bitmap_f13,	epd_bitmap_f14,	epd_bitmap_f15,	epd_bitmap_f16,	epd_bitmap_f17,	epd_bitmap_f18,	epd_bitmap_f19,	epd_bitmap_f20,	epd_bitmap_f21,	epd_bitmap_f22}; //store arrays in *archiGradStartupAnim

int numElementsStartupAnim = sizeof(archiGradStartupAnim) / sizeof(unsigned char *); //count how many items are in archiGradStartupAnim


// WEATHER ICONS
const unsigned char *symbol [] = {epd_bitmap_1, epd_bitmap_2, epd_bitmap_3, epd_bitmap_4, epd_bitmap_5, epd_bitmap_6, epd_bitmap_7, epd_bitmap_8, epd_bitmap_9, epd_bitmap_10, epd_bitmap_11, epd_bitmap_12, epd_bitmap_13};


// CUSTOM ANIM
const unsigned char *customAnim [] = {epd_bitmap_a_1, epd_bitmap_a_2,	epd_bitmap_a_3,	epd_bitmap_a_4,	epd_bitmap_a_5,	epd_bitmap_a_6,	epd_bitmap_a_7,	epd_bitmap_a_8,	epd_bitmap_a_9,  epd_bitmap_a_10,	epd_bitmap_a_11,	epd_bitmap_a_12,	epd_bitmap_a_13,	epd_bitmap_a_14,	epd_bitmap_a_15,	epd_bitmap_a_16,	epd_bitmap_a_17,	epd_bitmap_a_18,	epd_bitmap_a_19,	epd_bitmap_a_20}; //store arrays in *customAnim

int numElementsCustomAnim = sizeof(customAnim) / sizeof(unsigned char *); //count how many items are in customAnim



unsigned long LASTSTARTANIMTIME = 0;
unsigned long LASTSTARTHTTPREQUESTTIME = 0;
unsigned long LASTDAYREQUEST = 0;

#define LED_PIN 25

String APIKEY_OPENWEATHERMAP = "cb66a744575c1047a0991d78c6a32f01";

// const char* ssid = "INDA LAB";
// const char* password = "dex_2024";
// const char* ssid = "WiFi Fritzbox";
// const char* password = "jorisputteneers";

const char* ssid = "TP-Link_6B60";
const char* password = "Ikegcasen72.";


WebServer server(80);


unsigned long ota_progress_millis = 0;

// functions

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}



// intro logo animation
void startAnim(){
  static unsigned long lastExecutionTime = 0;
  unsigned long currentTime = millis();
    for(int j = 0; j< numElementsStartupAnim; j++){
      oled.drawBitmap(0, 0, archiGradStartupAnim[j], 128, 64, WHITE);  //reverse -> oled.drawBitmap(0, 0, archiGradStartupAnim[numElementsStartupAnim - (j+1)], 128, 64, WHITE);
      oled.display();
      oled.clearDisplay();
      delay(100);
    }
}


// custom animation
void customAnimation(){
  static unsigned long lastExecutionTime = 0;
  unsigned long currentTime = millis();
    for(int j = 0; j< numElementsCustomAnim; j++){
      oled.drawBitmap(0, 0, customAnim[j], 128, 64, WHITE);  //reverse -> oled.drawBitmap(0, 0, archiGradStartupAnim[numElementsStartupAnim - (j+1)], 128, 64, WHITE);
      oled.display();
      oled.clearDisplay();
      delay(100);
    }
}


// get day and date data measure once a day
void getDateAndTime(String serverNameDateAndTime) {

    // oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(1);
    HTTPClient http;

    // get time and date & day of week
    http.begin(serverNameDateAndTime.c_str());
    int httpResponseCodeDateAndTime = http.GET();
    if (httpResponseCodeDateAndTime == 200) { // Check if the request was successful
        String payloadDay = http.getString();
        Serial.println(payloadDay);

        DynamicJsonDocument jsonDateTime(2048); // Adjust the buffer size as needed
        DeserializationError errorA = deserializeJson(jsonDateTime, payloadDay);
        if (!errorA) {
          int8_t Day = jsonDateTime["day_of_week"];
          oled.setCursor(0, 10);
          // String weekDays[] = {"sundah", "moandah", "disndah", "oensdah", "dunderdah", "vridah", "zoaterdah"};
          // oled.println(weekDays[Day]);

          String dateTime = jsonDateTime["datetime"];
          int year, month, day, hour, minute, second;
          sscanf(dateTime.c_str(), "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &minute, &second);

           String formattedDate = String(day) + "/" + String(month) + "/" + String(year);
           String formattedTime = String(hour) + ":" + String(minute);

          oled.setCursor(0, 20);
          oled.println(formattedDate);
          oled.setCursor(0, 30);
          oled.println(formattedTime);
          oled.display();

          Serial.println(WiFi.localIP());

        }
        else {
            oled.println("JSON parsing error");
        }
    } else {
      oled.println("HTTP error: " + String(httpResponseCodeDateAndTime));
    }

    oled.display();
    http.end();
}




// generate a temperature from openweatherAPI measure every couple inutes
void getTempAndHumidityData(String serverNameTempAndHumidity) {

    // oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(1);

    HTTPClient http;    

    // get temperature and humidity
    http.begin(serverNameTempAndHumidity.c_str());
    int httpResponseCodeTempAndHumidity = http.GET();

    if (httpResponseCodeTempAndHumidity == 200) { // Check if the request was successful
        String payloadTempAndHumidity = http.getString();
        Serial.println(payloadTempAndHumidity);

        DynamicJsonDocument jsonTempHumid(1024); // Adjust the buffer size as needed
        DeserializationError errorB = deserializeJson(jsonTempHumid, payloadTempAndHumidity);

        
        if (!errorB) {
            float temperatureKelvin = jsonTempHumid["main"]["temp"]; // Extract the 
            float temperatureCelsius = temperatureKelvin - 273.15; // Convert to Celsius

            oled.setCursor(0, 0);
            oled.print("Din Daeng, Bangkok 10400 ");
            oled.setCursor(0, 40);
            oled.print("Ip: ");
            oled.print(WiFi.localIP()); // Display temperature with 2 decimal places
            // oled.print("C");


            // get description
            String description = jsonTempHumid["weather"][0]["description"]; 
            oled.setCursor(0, 50);
            oled.print("lon: 13.76020,  lat:100.55525");
            

            int randomIndex = random(sizeof(symbol) / sizeof(symbol[0]));

            // oled.drawBitmap(70, 10, symbol[randomIndex], 50, 50, WHITE);
            // oled.display();
            // oled.drawBitmap(70, 10, symbol[randomIndex], 50, 50, WHITE);
            // oled.println();
            // delay(1000);
        } else {
            oled.println("JSON parsing error");
        }
    } else {
        oled.println("HTTP error: " + String(httpResponseCodeTempAndHumidity));
    }

    oled.display();

    http.end();
}




void setup(void){
  Serial.begin(115200);

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  oled.clearDisplay();
  oled.setTextColor(WHITE);
	oled.setCursor(0, 10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
      oled.print(".");
      delay(100);
      oled.display();  
  }

  oled.clearDisplay();
	oled.setCursor(0, 10);
  oled.println("Connected to WiFi");
  delay(1000);
  oled.println(WiFi.localIP());
  oled.println("MAC: " + String(WiFi.macAddress()));
  oled.display();  
  
  delay(2000);   

  server.on("/", []() {
    server.send(200, "text/plain", "Hi! This is ElegantOTA Demo.");
  });

  ElegantOTA.begin(&server);    // Start ElegantOTA
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  server.begin();


  delay(2000);

  pinMode(LED_PIN, OUTPUT);


  startAnim();
  // customAnimation();
  getTempAndHumidityData("https://api.openweathermap.org/data/2.5/weather?lat=50.85&lon=2.88&appid=" + APIKEY_OPENWEATHERMAP);
  getDateAndTime("http://worldtimeapi.org/api/ip");
  
}

void loop(void) {
  server.handleClient();
  ElegantOTA.loop();

  unsigned long CURRENTTIME = millis();

  if (CURRENTTIME - LASTSTARTANIMTIME >= 600000) {
    customAnimation();
    LASTSTARTANIMTIME = CURRENTTIME;
  }

  if (CURRENTTIME - LASTSTARTHTTPREQUESTTIME >= 60000) {
    oled.clearDisplay();
    delay(100);
    getTempAndHumidityData("https://api.openweathermap.org/data/2.5/weather?lat=50.85&lon=2.88&appid=" + APIKEY_OPENWEATHERMAP);
    getDateAndTime("http://worldtimeapi.org/api/ip");
    LASTSTARTHTTPREQUESTTIME = CURRENTTIME;
  }


  // if (CURRENTTIME - LASTDAYREQUEST >= 4000) {
  //   getDateAndTime("http://worldtimeapi.org/api/ip");
  //   LASTDAYREQUEST = CURRENTTIME;
  // }

}











// correct weather information api rget request
// random location
// https://api.openweathermap.org/data/2.5/weather?lat=44.34&lon=10.99&appid=cb66a744575c1047a0991d78c6a32f01




// GENT
// https://api.openweathermap.org/data/2.5/weather?lat=51.05&lon=3.71667&appid=cb66a744575c1047a0991d78c6a32f01