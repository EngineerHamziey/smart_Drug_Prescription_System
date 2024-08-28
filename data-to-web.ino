#if 1

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Replace with your network credentials
const char* ssid = "project";
const char* password = "123456789";

// Data wire is plugged into pin 4 on the ESP32
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Pin for heart rate sensor
#define HEART_RATE_PIN 34

AsyncWebServer server(80);

float temperature = 0.0;
int heartRate = 0;

void setup() {
  // Start the serial communication
  Serial.begin(115200);
  
  // Start the Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize the DS18B20 sensor
  sensors.begin();

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Define routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  // Route to get temperature and heart rate
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"temperature\":" + String(temperature) + ", \"heartrate\":" + String(heartRate) + "}";
    request->send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  // Read temperature from DS18B20
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  // Read heart rate from HW-827
  heartRate = analogRead(HEART_RATE_PIN);

  // Add a delay for stability
  delay(2000);
}


#endif
