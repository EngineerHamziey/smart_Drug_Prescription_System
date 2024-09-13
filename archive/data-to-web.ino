#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "project";
const char* password = "123456789";

WebSocketsServer webSocket = WebSocketsServer(81);  // WebSocket server on port 81

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  Serial.println("WebSocket server started!");
}

void loop() {
  // Handle WebSocket events
  webSocket.loop();
}

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_CONNECTED:
      Serial.printf("[%u] Connected\n", num);
      webSocket.sendTXT(num, "Hello from ESP32 WebSocket Server");
      break;
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected\n", num);
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Text message: %s\n", num, payload);
      // Echo back the received message
      webSocket.sendTXT(num, payload, length);
      break;
    case WStype_BIN:
      Serial.printf("[%u] Binary message length: %u\n", num, length);
      break;
  }
}
