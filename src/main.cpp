#include <Arduino.h>
#include <PubSubClient.h>
#ifdef IHM
#include "LCDDisplay.h"
#include <Wire.h>
#include <Adafruit_SSD1351.h>
#endif
#ifdef CAPTEUR
#include "DHTSensor.h"
#endif

// WiFi credentials
const char* ssid = "your-ssid";
const char* password = "your-password";

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128

// Pin definitions
#define SCLK_PIN 4
#define MOSI_PIN 0
#define DC_PIN   2
#define CS_PIN   14
#define RST_PIN  12

// Create the SSD1351 display
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);

// Function prototypes
void drawTextAt(int x, int y, const char *text, uint16_t color);
void testlines(uint16_t color);
void tftPrintTest();

void setup() {
  Serial.begin(115200);
  Serial.print("\nhello!");
  tft.begin();

  Serial.println("init");
  tft.fillScreen(BLACK);

  uint16_t time = millis();
  time = millis() - time;
  delay(500);
  // Initialisation des différentes parties du système
  LCDDisplay::setupLCD();
  #ifdef CAPTEUR
    DHTSensor::setupDHT();
  #endif
}

void loop() {
  // Boucle principale
  LCDDisplay::handleLCD();
}


void drawTextAt(int x, int y, const char* text, uint16_t color) {
  tft.setCursor(x,y);
  tft.setTextColor(color);
  tft.print(text);
}
#ifdef TEST
void tftPrintTest() {
  tft.fillScreen(BLACK);
  tft.setCursor(0, 5);
  tft.setTextColor(RED);  
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(BLUE);
  tft.setTextSize(3);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 5);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(GREEN);
  tft.print("3.14");
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");
  tft.setTextColor(WHITE);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(WHITE);
  tft.print(" seconds.");
}
#endif

void testlines(uint16_t color) {
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width()-1; x+=6) {
     tft.drawLine(0, 0, x, tft.height()-1, color);
   }
   for (uint16_t y=0; y < tft.height()-1; y+=6) {
     tft.drawLine(0, 0, tft.width()-1, y, color);
   }
   
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width()-1; x+=6) {
     tft.drawLine(tft.width()-1, 0, x, tft.height()-1, color);
   }
   for (uint16_t y=0; y < tft.height()-1; y+=6) {
     tft.drawLine(tft.width()-1, 0, 0, y, color);
   }
   
   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width()-1; x+=6) {
     tft.drawLine(0, tft.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < tft.height()-1; y+=6) {
     tft.drawLine(0, tft.height()-1, tft.width()-1, y, color);
   }

   tft.fillScreen(BLACK);
   for (uint16_t x=0; x < tft.width()-1; x+=6) {
     tft.drawLine(tft.width()-1, tft.height()-1, x, 0, color);
   }
   for (uint16_t y=0; y < tft.height()-1; y+=6) {
     tft.drawLine(tft.width()-1, tft.height()-1, 0, y, color);
   }
   
}
