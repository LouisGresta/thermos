#ifndef LCD_DISPLAY_H
  #define LCD_DISPLAY_H

  #include <LiquidCrystal_I2C.h>

  class LCDDisplay {
  public:
    static void setupLCD();
    static void handleLCD();

  private:
    static LiquidCrystal_I2C lcd;
    static void updateDisplay(float temperature, float humidity);
  };

#endif