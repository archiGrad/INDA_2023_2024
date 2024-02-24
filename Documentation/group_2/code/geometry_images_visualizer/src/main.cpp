

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
const unsigned char *archiGradStartupAnim [] = {epd_bitmap_0001,	epd_bitmap_0002,	epd_bitmap_0003,	epd_bitmap_0004,	epd_bitmap_0005,	epd_bitmap_0006,	epd_bitmap_0007,	epd_bitmap_0008,	epd_bitmap_0009,  epd_bitmap_0010,	epd_bitmap_0011,	epd_bitmap_0012,	epd_bitmap_0013,	epd_bitmap_0014,	epd_bitmap_0015,	epd_bitmap_0016,	epd_bitmap_0017,	epd_bitmap_0018,	epd_bitmap_0019,	epd_bitmap_0020,  epd_bitmap_0020,	epd_bitmap_0021,	epd_bitmap_0022,	epd_bitmap_0023,	epd_bitmap_0024,	epd_bitmap_0025,	epd_bitmap_0026,	epd_bitmap_0027,	epd_bitmap_0028,	epd_bitmap_0029,	epd_bitmap_0030,	epd_bitmap_0031,	epd_bitmap_0032,	epd_bitmap_0033,	epd_bitmap_0034,	epd_bitmap_0035,	epd_bitmap_0036,	epd_bitmap_0037,	epd_bitmap_0038,	epd_bitmap_0039,	epd_bitmap_0040, 	epd_bitmap_0041,	epd_bitmap_0042,	epd_bitmap_0043,	epd_bitmap_0044,	epd_bitmap_0045,	epd_bitmap_0046,	epd_bitmap_0047,	epd_bitmap_0048,	epd_bitmap_0049, epd_bitmap_0050}; //store arrays in *archiGradStartupAnim

int numElementsStartupAnim = sizeof(archiGradStartupAnim) / sizeof(unsigned char *); //count how many items are in archiGradStartupAnim


// WEATHER ICONS
const unsigned char *symbol [] = {epd_bitmap_0001,	epd_bitmap_0002,	epd_bitmap_0003,	epd_bitmap_0004,	epd_bitmap_0005,	epd_bitmap_0006,	epd_bitmap_0007,	epd_bitmap_0008,	epd_bitmap_0009,  epd_bitmap_0010,	epd_bitmap_0011,	epd_bitmap_0012,	epd_bitmap_0013,	epd_bitmap_0014,	epd_bitmap_0015,	epd_bitmap_0016,	epd_bitmap_0017,	epd_bitmap_0018,	epd_bitmap_0019,	epd_bitmap_0020,  epd_bitmap_0020,	epd_bitmap_0021,	epd_bitmap_0022,	epd_bitmap_0023,	epd_bitmap_0024,	epd_bitmap_0025,	epd_bitmap_0026,	epd_bitmap_0027,	epd_bitmap_0028,	epd_bitmap_0029,	epd_bitmap_0030,	epd_bitmap_0031,	epd_bitmap_0032,	epd_bitmap_0033,	epd_bitmap_0034,	epd_bitmap_0035,	epd_bitmap_0036,	epd_bitmap_0037,	epd_bitmap_0038,	epd_bitmap_0039,	epd_bitmap_0040, 	epd_bitmap_0041,	epd_bitmap_0042,	epd_bitmap_0043,	epd_bitmap_0044,	epd_bitmap_0045,	epd_bitmap_0046,	epd_bitmap_0047,	epd_bitmap_0048,	epd_bitmap_0049, epd_bitmap_0050};


// CUSTOM ANIM
const unsigned char *customAnim [] = {epd_bitmap_0001,	epd_bitmap_0002,	epd_bitmap_0003,	epd_bitmap_0004,	epd_bitmap_0005,	epd_bitmap_0006,	epd_bitmap_0007,	epd_bitmap_0008,	epd_bitmap_0009,  epd_bitmap_0010,	epd_bitmap_0011,	epd_bitmap_0012,	epd_bitmap_0013,	epd_bitmap_0014,	epd_bitmap_0015,	epd_bitmap_0016,	epd_bitmap_0017,	epd_bitmap_0018,	epd_bitmap_0019,	epd_bitmap_0020,  epd_bitmap_0020,	epd_bitmap_0021,	epd_bitmap_0022,	epd_bitmap_0023,	epd_bitmap_0024,	epd_bitmap_0025,	epd_bitmap_0026,	epd_bitmap_0027,	epd_bitmap_0028,	epd_bitmap_0029,	epd_bitmap_0030,	epd_bitmap_0031,	epd_bitmap_0032,	epd_bitmap_0033,	epd_bitmap_0034,	epd_bitmap_0035,	epd_bitmap_0036,	epd_bitmap_0037,	epd_bitmap_0038,	epd_bitmap_0039,	epd_bitmap_0040, 	epd_bitmap_0041,	epd_bitmap_0042,	epd_bitmap_0043,	epd_bitmap_0044,	epd_bitmap_0045,	epd_bitmap_0046,	epd_bitmap_0047,	epd_bitmap_0048,	epd_bitmap_0049, epd_bitmap_0050}; //store arrays in *customAnim

int numElementsCustomAnim = sizeof(customAnim) / sizeof(unsigned char *); //count how many items are in customAnim



unsigned long LASTSTARTANIMTIME = 0;
unsigned long LASTSTARTHTTPREQUESTTIME = 0;
unsigned long LASTDAYREQUEST = 0;

#define LED_PIN 25

String APIKEY_OPENWEATHERMAP = "cb66a744575c1047a0991d78c6a32f01";

const char* ssid = "INDA LAB";
const char* password = "dex_2024";
// const char* ssid = "WiFi Fritzbox";
// const char* password = "jorisputteneers";




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
          String weekDays[] = {"sundah", "moandah", "disndah", "oensdah", "dunderdah", "vridah", "zoaterdah"};
          oled.println(weekDays[Day]);

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
            oled.print("Vlamertinge ");
            oled.setCursor(0, 40);
            oled.print("Temp: ");
            oled.print(temperatureCelsius, 1); // Display temperature with 2 decimal places
            oled.print("C");


            // get description
            String description = jsonTempHumid["weather"][0]["description"]; 
            oled.setCursor(0, 50);
            oled.print(description);
            

            int randomIndex = random(sizeof(symbol) / sizeof(symbol[0]));

            oled.drawBitmap(70, 10, symbol[randomIndex], 50, 50, WHITE);
            oled.display();
            oled.drawBitmap(70, 10, symbol[randomIndex], 50, 50, WHITE);
            oled.println();
            delay(1000);
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