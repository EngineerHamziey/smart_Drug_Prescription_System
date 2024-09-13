#if 0

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define HEART_RATE_PIN A0  // ADC pin for heart rate sensor
#define THRESHOLD 550      // Threshold to detect heartbeat signal
#define LED_PIN 2          // Optional LED to blink with each heartbeat

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Initialize LCD with I2C address and dimensions

// Variables to store heart rate calculation
unsigned long lastBeatTime = 0;
unsigned int beatsPerMinute = 0;
int beatCount = 0;
int lastSignal = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Heart Rate Monitor");

  // Set LED pin as output (optional)
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int signal = analogRead(HEART_RATE_PIN);  // Read the heart rate sensor's analog value

  // Detect rising edge (heartbeat)
  if (signal > THRESHOLD && lastSignal <= THRESHOLD) {
    unsigned long currentTime = millis();
    unsigned long timeDifference = currentTime - lastBeatTime;

    // Calculate BPM if more than 300ms passed since the last beat
    if (timeDifference > 300) {
      beatsPerMinute = 60000 / timeDifference;  // Calculate BPM (60000 ms in one minute)
      lastBeatTime = currentTime;
      beatCount++;

      // Blink LED on heartbeat (optional)
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
    }
  }

  lastSignal = signal;  // Store the current signal as the last signal for edge detection

  // Display the heart rate on the LCD
  lcd.setCursor(0, 1);
  lcd.print("BPM: ");
  lcd.print(beatsPerMinute);

  // Send signal data to Serial Plotter (optional for debugging)
  Serial.print("Signal: ");
  Serial.print(signal);
  Serial.print("\tBPM: ");
  Serial.println(beatsPerMinute);

  delay(20);  // Small delay for stability
}

#endif