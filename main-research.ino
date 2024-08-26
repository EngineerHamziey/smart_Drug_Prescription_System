#if 1

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// WiFi credentials
const char* ssid = "project";
const char* password = "123456789";

// Initialize the web server
AsyncWebServer server(80);

// Sensor pins
const int oneWireBus = 5;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Array for storing answers
String answers[12];
int currentQuestion = 0;

void setup() {
  Serial.begin(115200);

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("Failed to mount file system");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // Serve the index.html file
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Serve CSS and JS files
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
  
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  // Handle form submission
  server.on("/answer", HTTP_GET, [](AsyncWebServerRequest *request){
    String answer = request->getParam("answer")->value();
    answers[currentQuestion] = answer;
    currentQuestion++;
    if (currentQuestion < 12) {
      request->send(SPIFFS, "/index.html", "text/html");
    } else {
      request->send(200, "text/html", "Thank you for answering the questions. Processing your information...");
      checkVitals();
    }
  });

  // Start the server
  server.begin();
}

void checkVitals() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  // Replace with actual heart rate check logic
  int heartRate = 80; // Example value

  if (temperature < 36.1 || temperature > 37.2 || heartRate < 60 || heartRate > 100) {
    sendMessage("Please see a doctor.");
  } else {
    sendMessage("Your vitals are normal.");
  }
}

void sendMessage(String message) {
  // Implement your message sending logic here
  // E.g., send an email or display it on a web page
}

void loop() {
  // No need for anything in the loop for now
}


#endif