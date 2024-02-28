#ifndef AFFICHEUR_H
#define AFFICHEUR_H

#ifdef OLED
#include <Adafruit_SSD1351.h>

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

// Text dimensions
#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define NB_CHARS SCREEN_WIDTH / CHAR_WIDTH
#define NB_LINES SCREEN_HEIGHT / CHAR_HEIGHT

// Pin definitions
#define SCLK_PIN 4
#define MOSI_PIN 0
#define DC_PIN   2
#define CS_PIN   14
#define RST_PIN  12

extern Adafruit_SSD1351 tft;

// Function prototypes
void setupTFT();
void drawTextAt(int x, int y, const char *text, uint16_t color);
void clearLine(int y, int charHeight = CHAR_HEIGHT);
void testlines(uint16_t color);
void tftPrintTest();
#endif

#endif // AFFICHEUR_H