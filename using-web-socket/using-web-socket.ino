#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>

// Replace with your network credentials
const char* ssid = "project";
const char* password = "123456789";

// Data wire is plugged into pin 5 on the ESP32
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pin for heart rate sensor
#define HEART_RATE_PIN 32

// Create an instance of the PulseSensorPlayground
PulseSensorPlayground pulseSensor;

// WebSocket server on port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Create an instance of the LCD (20x4 display)
LiquidCrystal_I2C lcd(0x27, 20, 4);

float temperature = 0.0;
int heartRate = 0;
String ipAddress = "";

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting to WiFi...");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Get the IP address
  ipAddress = WiFi.localIP().toString();
  Serial.println("IP Address: " + ipAddress);

  // Display the IP address on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP:");
  lcd.setCursor(3, 0);
  lcd.print(ipAddress);

  // Start the WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Initialize the DS18B20 sensor
  sensors.begin();

  // Initialize the PulseSensor
  pulseSensor.analogInput(HEART_RATE_PIN);
  pulseSensor.begin();
}

// Function to handle WebSocket events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.printf("[%u] Connected\n", num);
  } else if (type == WStype_TEXT) {
    Serial.printf("[%u] Received: %s\n", num, payload);
  }
}

void loop() {
  // Handle WebSocket communication
  webSocket.loop();

  // Read temperature from DS18B20
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  if (temperature < 0) {
    temperature = 0;
  }

  // Simulate a random heart rate (or read from the actual sensor)
  heartRate = random(60, 73);

  // Update the LCD with temperature and heart rate data
  lcd.setCursor(0, 1);  // Row 1
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print("C  ");
  
  lcd.setCursor(0, 2);  // Row 2
  lcd.print("HR:");
  lcd.print(heartRate);
  lcd.print(" BPM ");

  // Send the heart rate and temperature data via WebSocket
  String data = String("{\"heartRate\":") + heartRate + ",\"temperature\":" + temperature + "}";
  webSocket.broadcastTXT(data);

  delay(1000); // Send data and update every second
}
