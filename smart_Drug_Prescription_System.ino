#if 0

#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>

// temperature sensor is plugged into pin 5 on the ESP32
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pin for heart rate sensor
#define HEART_RATE_PIN 32

// Create an instance of the PulseSensorPlayground
PulseSensorPlayground pulseSensor;

const char* ssid = "love";         // Your WiFi SSID
const char* password = "olamidesamsig";   // Your WiFi Password

WebSocketsServer webSocket = WebSocketsServer(81);  // WebSocket server on port 81

// Create an instance of the LCD (20x4 display)
LiquidCrystal_I2C lcd(0x27, 20, 4);


float temperature = 0.0;
int heartRate = 0;
String ipAddress = "";

void handleWebSocketMessage(uint8_t * payload, size_t length) {
  // No need to handle incoming messages for this application
}

void onEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("Client %u connected\n", client_num);
      break;
    case WStype_DISCONNECTED:
      Serial.printf("Client %u disconnected\n", client_num);
      break;
    case WStype_TEXT:
      handleWebSocketMessage(payload, length);
      break;
  }
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

void setup() {// Start the serial communication
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to WiFi...");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
    lcd.clear();
    delay(500);
    lcd.print("WiFi connecting....");
  }
  Serial.println("Connected to WiFi");
  lcd.clear();
  lcd.print("Connected to WiFi");
  delay(1000);

  // Get the IP address
  ipAddress = WiFi.localIP().toString();
  Serial.println("IP Address: " + ipAddress);

  // Display the IP address on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP:");
  lcd.setCursor(3, 0);
  lcd.print(ipAddress);

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(onEvent);
  // Initialize the DS18B20 sensor
  sensors.begin();

  // Initialize the PulseSensor
  pulseSensor.analogInput(HEART_RATE_PIN);
  pulseSensor.begin();
}

void updateLcd() {
  // Update the LCD with temperature and heart rate data
  lcd.setCursor(0, 2);  // Row 1
  lcd.print("Temperature: ");
  lcd.print(temperature);
  lcd.print("C  ");
  
  lcd.setCursor(0, 3);
  lcd.print("Heart Rate: ");
  lcd.print(heartRate);
  lcd.print(" BPM");
}

void loop() {
  webSocket.loop();


  // Read temperature from DS18B20
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  if (temperature < 0) {
    temperature = 0;
  }

  if (touchDetected()) {
    int targetHeartRate = random(60, 73);

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

    // Send the heart rate and temperature data via WebSocket
    String data = String("{\"heartRate\":") + targetHeartRate + ",\"temperature\":" + temperature + "}";
    webSocket.broadcastTXT(data);
    Serial.println("Sent: " + data);
    // delay(2000); // Send new random numbers every 5 seconds

    heartRate = targetHeartRate;
    updateLcd();

    // Stop further execution
    while (true) {
      webSocket.loop();
      webSocket.broadcastTXT(data);
      
      delay(2000); // Infinite loop to stop further execution
    }
  }
  updateLcd();

  

  // Send the heart rate and temperature data via WebSocket
  String data = String("{\"heartRate\":") + heartRate + ",\"temperature\":" + temperature + "}";
  webSocket.broadcastTXT(data);
  Serial.println("Sent: " + data);
  delay(300); // Send new random numbers every 5 seconds
}

#endif