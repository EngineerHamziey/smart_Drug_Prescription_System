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

LiquidCrystal_I2C lcd(0x27, 20, 4);

float temperature = 0.0;
int heartRate = 0;

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

  // Display the project title on the LCD
  lcd.setCursor(0, 0);
  lcd.print("SMART DRUG");
  lcd.setCursor(0, 1);
  lcd.print("PRESCRIPTION SYSTEM");

  delay(3000);  // Delay to show the title for 3 seconds

  // Clear the screen before displaying data
  // lcd.clear();
}

void loop() {
  // Read temperature from DS18B20
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  temperature = (temperature < 0) ? 0 : temperature;

  // Update the PulseSensor
  int pulse = pulseSensor.getBeatsPerMinute();
  
  // Check if pulse value is valid
  Serial.println(analogRead(HEART_RATE_PIN));
  if (pulse > 0) {
    heartRate = pulse;
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
  delay(2000);
}

#endif