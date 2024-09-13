#if 0
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin for heart rate sensor
#define HEART_RATE_PIN 32

// Create an instance of the LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Display initial message on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Touch Detection");
}

// Function to detect touch
bool touchDetected() {
  int sensorValue = analogRead(HEART_RATE_PIN);
  // Log the sensor value to the serial monitor
  Serial.print("Analog Value: ");
  Serial.println(sensorValue);
  // Return true if touch is detected, false otherwise
  return sensorValue <= 3;
}

void loop() {
  // Check if touch is detected
  if (touchDetected()) {
    Serial.println("Touch Detected!");
    lcd.setCursor(0, 1);
    lcd.print("Touch Detected! ");
  } else {
    Serial.println("No Touch");
    lcd.setCursor(0, 1);
    lcd.print("No Touch       "); // Clear the previous message
  }

  // Add a delay for stability
  delay(500);
}
#endif