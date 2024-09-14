#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "project";         // Your WiFi SSID
const char* password = "123456789";   // Your WiFi Password

WebSocketsServer webSocket = WebSocketsServer(81);  // WebSocket server on port 81

void handleWebSocketMessage(uint8_t * payload, size_t length) {
  // incoming messages from the client can be handled here
  // payload is the text message while length is the length of the text
}

// client_num, all clients are assigned a unique number and that is used to send re
void onEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  // The onEvent function monitors the different WebSocket events: when a client connects, disconnects, or sends a text message
  switch (type) {
    case WStype_CONNECTED:
      Serial.printf("Client %u connected\n", client_num);
      break;
    case WStype_DISCONNECTED:
      Serial.printf("Client %u disconnected\n", client_num);
      break;
    case WStype_TEXT: // if message was received from client
      handleWebSocketMessage(payload, length);// handle the messages received
      break;
  }
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP()); // This is the IP you will enter into the website

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(onEvent);
}

void generateRandomNumbers() {
  // Generate two random numbers
  int randomNum1 = random(0, 100);
  int randomNum2 = random(0, 100);
}

void sendJSON() {
  // Send random numbers as JSON to all connected clients
  String message = "{\"random1\": " + String(randomNum1) + ", \"random2\": " + String(randomNum2) + "}";
  webSocket.broadcastTXT(message);// send to all clients

  Serial.println("Sent: " + message);
}

void loop() {
  webSocket.loop();

  generateRandomNumbers();
  sendJSON();

  delay(2000); // Send new random numbers every 5 seconds
}
