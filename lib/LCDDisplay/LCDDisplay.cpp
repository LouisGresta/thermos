#include "LCDDisplay.h"
#include "MQTTConnection.h"

LiquidCrystal_I2C LCDDisplay::lcd(0x27, 16, 2);

void LCDDisplay::setupLCD() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
}

void LCDDisplay::handleLCD() {
  // Exemple de récupération des données depuis MQTT
  float temperature, humidity;
  // Récupérer les données de température et d'humidité depuis MQTT
  // ...
  // Mettre à jour l'affichage LCD
  updateDisplay(temperature, humidity);
}

void LCDDisplay::updateDisplay(float temperature, float humidity) {
  lcd.setCursor(12, 0);
  lcd.print(temperature);
  lcd.setCursor(10, 1);
  lcd.print(humidity);
}