
#if 0

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
// required libraries for web server
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C myLcd(0x27,16,4);

#define displayDelay 2500
#define characterDelay 80

void initializeLCD() {
  Wire.begin();
  myLcd.init();
  myLcd.backlight();
  myLcd.setCursor(0, 0);
  myLcd.clear();
}


void printLoading(int startXPosition,  int yPostion, int endXPosition) {
  byte loadCharacter[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
  };
  myLcd.createChar(0, loadCharacter);
  myLcd.setCursor(startXPosition, yPostion);
  //  = 0;
  for (int count = 0; count <= endXPosition; count++) {
    myLcd.write((uint8_t)0); // Write the custom character (character index 0, i.e the first custome character)
    delay(characterDelay);
  }
}

void printTitle() {
  myLcd.clear();
  delay(100);
  printInTypingEffect("BLOCKCHAIN ENABLED", 1, 0);
  printInTypingEffect("SMART ELECTRIC", 3, 1);
  printInTypingEffect("POWER METER", 4, 2);
}

void printInTypingEffect(String text, int xColumn, int yRow) {
  myLcd.setCursor(xColumn, yRow);
  for (int letterIndex = 0; letterIndex < text.length(); letterIndex++) {
    myLcd.print(text.charAt(letterIndex));
    delay(characterDelay);
  }
}


void doAllInitialization() {
  initializeLCD();
  printTitle();
  printLoading(0, 3, 19);
  beginTheSerialMonitor();
}

void setup() {
  doAllInitialization();
  delay(1000);

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  beginTheSerialMonitor();
  
  WiFiManager myWifiManager;//creating object to represent our wifi manager

  myLcd.clear();
  printInTypingEffect("  CONFIGURING WIFI", 0, 0);
  bool res = myWifiManager.autoConnect("Blockchain Power Meter","password"); // password protected app

  if(!res) {
    Serial.println("Failed to connect");
    printInTypingEffect(" CONNECTION FAILED", 0, 2);
  } 
  else {
    Serial.println("connected...yeey :)");
    printInTypingEffect("   WIFI CONNECTED", 0, 2);
    printInTypingEffect("    SUCCESSFULLY", 0, 3);
  }

}

void loop() {
    // put your main code here, to run repeatedly:   
}

#endif
