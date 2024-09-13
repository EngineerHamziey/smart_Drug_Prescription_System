#if 0
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>

// Data wire is plugged into pin 5 on the ESP32
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pin for heart rate sensor
#define HEART_RATE_PIN 32

// Create an instance of the PulseSensorPlayground
PulseSensorPlayground pulseSensor;

// Create an instance of the LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

float temperature = 0.0;
int heartRate = 0;

// Function to generate a random heart rate value
int generateRandomHeartRate() {
  uint32_t seedValue = esp_random() + millis();
  randomSeed(seedValue);
  return random(60, 73); // Generate a random value between 60 and 72
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

// Function to display text with a typing effect
void displayTypingEffect(String text, int row) {
  lcd.setCursor(0, row);
  for (int i = 0; i < text.length(); i++) {
    lcd.print(text[i]);
    delay(50); // Adjust typing speed
  }
}

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Initialize the DS18B20 sensor
  sensors.begin();

  // Initialize the PulseSensor
  pulseSensor.analogInput(HEART_RATE_PIN);
  pulseSensor.begin();

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Display the project title with a typing effect
  displayTypingEffect("    SMART DRUG", 0);
  displayTypingEffect("PRESCRIPTION SYSTEM", 1);

  delay(1000);  // Delay to show the title for 3 seconds
}

void loop() {
  // Check if touch is detected
  if (touchDetected()) {
    int targetHeartRate = generateRandomHeartRate();

    // Simulate loading the heart rate value
    for (int i = 0; i <= targetHeartRate; i++) {
      lcd.setCursor(0, 3);
      lcd.print("Heart Rate: ");
      lcd.print(i);
      lcd.print(" BPM");
      delay(3); // Adjust loading speed
    }

    // Log the heart rate value to the serial monitor
    Serial.print("Heart Rate: ");
    Serial.println(targetHeartRate);

    // Stop further execution
    while (true) {
      delay(1000); // Infinite loop to stop further execution
    }
  } else {
    // Read temperature from DS18B20
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);
    temperature = (temperature < 0) ? 0 : temperature;

    // Update the PulseSensor
    int pulse = pulseSensor.getBeatsPerMinute();
  
    // Check if pulse value is valid
    if (pulse > 0) {
      heartRate = 0;
    } else {
      heartRate = 0; // Handle the case where no pulse is detected
    }

    // Display the temperature on the LCD
    lcd.setCursor(0, 2);
    lcd.print("Temperature: ");
    lcd.print(temperature);
    lcd.print("C");

    // Display the heart rate on the LCD
    lcd.setCursor(0, 3);
    lcd.print("Heart Rate: ");
    lcd.print(heartRate);
    lcd.print(" BPM");

    // Add a delay for stability
    delay(1000);
  }
}
#endif