

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
#include <WiFi.h>



#include <images.h> //get images.h file,Tu here i store my img data.S


// define screen settings
#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// images to memory conversion here -> http://javl.github.io/image2cpp/

// const unsigned char *archiGradStartupAnim [] = {epd_bitmap_0001,	epd_bitmap_0002,	epd_bitmap_0003,	epd_bitmap_0004,epd_bitmap_0005}; //store arrays in *archiGradStartupAnim

// int numElementsStartupAnim = sizeof(archiGradStartupAnim) / sizeof(unsigned char *); //count how many items are in archiGradStartupAnim



// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 26400)
const int numElementsStartupAnim = 50;
const unsigned char* archiGradStartupAnim[50] = {
	epd_bitmap_0001,
	epd_bitmap_0002,
	epd_bitmap_0003,
	epd_bitmap_0004,
	epd_bitmap_0005,
	epd_bitmap_0006,
	epd_bitmap_0007,
	epd_bitmap_0008,
	epd_bitmap_0009,
	epd_bitmap_0010,
	epd_bitmap_0011,
	epd_bitmap_0012,
	epd_bitmap_0013,
	epd_bitmap_0014,
	epd_bitmap_0015,
	epd_bitmap_0016,
	epd_bitmap_0017,
	epd_bitmap_0018,
	epd_bitmap_0019,
	epd_bitmap_0020,
	epd_bitmap_0021,
	epd_bitmap_0022,
	epd_bitmap_0023,
	epd_bitmap_0024,
	epd_bitmap_0025,
	epd_bitmap_0026,
	epd_bitmap_0027,
	epd_bitmap_0028,
	epd_bitmap_0029,
	epd_bitmap_0030,
	epd_bitmap_0031,
	epd_bitmap_0032,
	epd_bitmap_0033,
	epd_bitmap_0034,
	epd_bitmap_0035,
	epd_bitmap_0036,
	epd_bitmap_0037,
	epd_bitmap_0038,
	epd_bitmap_0039,
	epd_bitmap_0040,
	epd_bitmap_0041,
	epd_bitmap_0042,
	epd_bitmap_0043,
	epd_bitmap_0044,
	epd_bitmap_0045,
	epd_bitmap_0046,
	epd_bitmap_0047,
	epd_bitmap_0048,
	epd_bitmap_0049,
	epd_bitmap_0050
};



unsigned long LASTSTARTANIMTIME = 0;
unsigned long LASTSTARTHTTPREQUESTTIME = 0;
unsigned long LASTDAYREQUEST = 0;

#define LED_PIN 25

// functions

// intro logo animation
void startAnim(){
  static unsigned long lastExecutionTime = 0;
  unsigned long currentTime = millis();
    for(int j = 0; j< numElementsStartupAnim; j++){
      oled.drawBitmap(32, 0, archiGradStartupAnim[j], 64, 64, WHITE);  
      oled.display();
      oled.clearDisplay();
      delay(random(10, 1001));
    }
}

void setup(){
  Serial.begin(115200);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  // oled.clearDisplay();
  // oled.setTextColor(WHITE);
	// oled.setCursor(10, 10);
  // oled.println("geometry images");
  pinMode(LED_PIN, OUTPUT);
  // startAnim();
  
}

void loop() {
  // delay(100);
  
  for(int i = 0; i< 100; i++){
    startAnim();
    // startAnim();
    // startAnim();
    oled.display();

  }
  // // oled.clearDisplay();
  // oled.setTextColor(WHITE);
	// oled.setCursor(70, 10);
  // oled.println("geometry");
	// oled.setCursor(70, 20);
  // oled.println("images");
  // delay(100);

}


