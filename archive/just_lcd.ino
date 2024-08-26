#if 0
// esp32 sda = gpio 21, while it's scl = 22

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C myLcd(0x27,16,4);

#define displayDelay 2500


void initializeLCD() {
  Wire.begin();
  myLcd.init();
  myLcd.backlight();
  myLcd.setCursor(0, 0);
  myLcd.clear();
}

void printTitle() {
  myLcd.clear();
  delay(500);
  printInTypingEffect("  IOT ENABLED DRUG  ", 0, 0, 150);
  myLcd.setCursor(0, 2);
  printInTypingEffect("PRESCRIPTION SYSTEM", 1, 2, 150);
  delay(displayDelay);
}

void printInTypingEffect(String text, int xColumn, int yRow, int eachDelay) {
  myLcd.setCursor(xColumn, yRow);
  for (int letterIndex = 0; letterIndex < text.length(); letterIndex++) {
    myLcd.print(text.charAt(letterIndex));
    delay(eachDelay);
  }
}


void doAllInitialization() {
  initializeLCD();
  printTitle();
  beginTheSerialMonitor();
}

void setup() {
  doAllInitialization();
  delay(1000);
}



void loop() {
  delay(500);
}

#endif