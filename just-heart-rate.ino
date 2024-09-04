#if 0

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin for heart rate sensor
#define HEART_RATE_PIN 32

// Variables for heart rate detection
int signalValue = 0;        // Holds the raw analog value from the sensor
int threshold = 550;        // Threshold value to detect heartbeats, adjust as needed
unsigned long lastBeatTime = 0; // Time of the last beat detected
int beatCount = 0;          // Number of beats detected
float heartRate = 0.0;      // Calculated heart rate in BPM

// LCD display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Display the project title on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Heart Rate Monitor");

  delay(2000);  // Delay to show the title for 2 seconds

  // Clear the screen before displaying data
  lcd.clear();
}

void loop() {
  // Read the analog value from the heart rate sensor
  signalValue = analogRead(HEART_RATE_PIN);

  // Detect a beat when the signal exceeds the threshold
  if (signalValue > threshold) {
    unsigned long currentTime = millis();
    if (currentTime - lastBeatTime > 300) { // Minimum time between beats to avoid noise
      // Calculate heart rate in BPM
      heartRate = 60000.0 / (currentTime - lastBeatTime);
      lastBeatTime = currentTime;
      beatCount++;
    }
  }

  // Display the raw signal value on the Serial Monitor for debugging
  Serial.print("Signal Value: ");
  Serial.println(signalValue);

  // Display the heart rate on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Heart Rate: ");
  if (heartRate > 0) {
    Serial.println("heart rate = " + String(heartRate));
    lcd.print(heartRate);
    lcd.print(" BPM");
  } else {
    lcd.print("No Pulse");
  }•••••••••••

  // Add a delay for stability (adjust as needed)
  delay(200);
}

#endif