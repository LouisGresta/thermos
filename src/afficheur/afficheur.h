#ifndef AFFICHEUR_H
    #define AFFICHEUR_H

    #ifdef IHM
        #include <Wire.h>
        #include <Arduino.h>
        #include <LiquidCrystal_I2C.h>

        //Adresse I2C DE L'ecran LCD
        #define LCD_ADDRESS 0X27


        //Nombres de colonnes et de lignes de l'ecran LCD
        #define LCD_COLUMNS 16
        #define LCD_ROWS 2


        void setupLCD();

        void displayMessage(const char* message);
    #endif

    #ifdef OLED
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
    void setupTFT(Adafruit_SSD1351 tft);
    void drawTextAt(int x, int y, const char *text, uint16_t color);
    void testlines(uint16_t color);
    void tftPrintTest();
    #endif

#endif // AFFICHEUR_H