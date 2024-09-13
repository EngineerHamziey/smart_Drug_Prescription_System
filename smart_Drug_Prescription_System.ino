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

const char* ssid = "project";         // Your WiFi SSID
const char* password = "123456789";   // Your WiFi Password

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

void loop() {
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
  lcd.print(" C  ");
  
  lcd.setCursor(0, 2);  // Row 2
  lcd.print("HR:");
  lcd.print(heartRate);
  lcd.print(" BPM ");

  // Send the heart rate and temperature data via WebSocket
  String data = String("{\"heartRate\":") + heartRate + ",\"temperature\":" + temperature + "}";
  webSocket.broadcastTXT(data);
  Serial.println("Sent: " + data);
  delay(2000); // Send new random numbers every 5 seconds
}
