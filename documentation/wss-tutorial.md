Here’s the documentation in Markdown format with added explanations and line-by-line code comments:

---

# Switching from `ws` to `wss` on ESP32 using SSL/TLS

To change the WebSocket connection from **ws** (WebSocket) to **wss** (WebSocket Secure), we need to establish a secure connection via **TLS** (Transport Layer Security). However, the ESP32 WebSocket libraries, such as `WebSocketsServer`, do not natively support **wss**. 

This guide will show you how to configure a secure WebSocket using the **ESPAsyncWebServer** with **SSL** support and how to set up certificates for secure communication.

---

## Steps to Switch to Secure WebSockets (wss)

### 1. Use a Secure WebSocket Library

Since `WebSocketsServer` does not support SSL/TLS, we will use `ESPAsyncWebServer` which has SSL support.

### 2. Implement SSL/TLS Certificates

SSL/TLS certificates are required to encrypt communication. You can generate self-signed certificates or get them from a trusted Certificate Authority (CA). These certificates will be uploaded to the ESP32's **SPIFFS** (SPI Flash File System).

### 3. Code Adjustments

Below is a code example that demonstrates how to implement a secure WebSocket using **WiFiClientSecure** and **AsyncWebSocket**.

---

### Code Example

```cpp
#include <WiFi.h>                      // For connecting to WiFi
#include <WiFiClientSecure.h>           // For SSL/TLS support
#include <ESPAsyncWebServer.h>          // Async Web Server library
#include <AsyncWebSocket.h>             // For WebSocket support
#include <FS.h>                         // File system support
#include <SPIFFS.h>                     // SPIFFS for storing certificate and key

// Define WiFi credentials
const char* ssid = "project";           // WiFi network SSID
const char* password = "123456789";     // WiFi network password

// Secure WebSocket server on port 443 (HTTPS/WSS)
AsyncWebServer server(443);             
AsyncWebSocket ws("/ws");               // WebSocket endpoint at /ws

// Path to the SSL/TLS certificate and private key stored in SPIFFS
const char *certPath = "/serverCert.pem";   
const char *privateKeyPath = "/privateKey.pem";

// Secure WiFi client for SSL/TLS
WiFiClientSecure client;

void setup() {
  Serial.begin(115200);                 // Initialize serial communication
  WiFi.begin(ssid, password);           // Connect to WiFi network
  
  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected");

  // Initialize SPIFFS for storing certificates
  if (!SPIFFS.begin()) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }

  // Load certificate and private key from SPIFFS
  File cert = SPIFFS.open(certPath, "r");          // Open certificate file
  File privateKey = SPIFFS.open(privateKeyPath, "r");  // Open private key file

  if (!cert || !privateKey) {
    Serial.println("Failed to open cert or key file.");
    return;
  }

  // Load the SSL certificates into the client
  client.loadCertificate(cert);         // Load SSL certificate
  client.loadPrivateKey(privateKey);    // Load private key

  // Close the files after loading
  cert.close();
  privateKey.close();

  // Set up WebSocket event handling
  ws.onEvent(onEvent);                  // Register WebSocket event handler
  server.addHandler(&ws);               // Attach WebSocket handler to server
  
  // Start the secure WebSocket server
  server.beginSecure(certPath, privateKeyPath); 
}

void loop() {
  // Cleanup inactive WebSocket clients
  ws.cleanupClients(); 
}

// WebSocket event handler
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("Client %u connected\n", client->id());
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("Client %u disconnected\n", client->id());
  }
}
```

### Explanation of the Code

```cpp
#include <WiFi.h>                      
#include <WiFiClientSecure.h>           
#include <ESPAsyncWebServer.h>          
#include <AsyncWebSocket.h>             
#include <FS.h>                         
#include <SPIFFS.h>                     
```
- **WiFi.h**: Manages WiFi connection on ESP32.
- **WiFiClientSecure.h**: Provides SSL/TLS support for secure connections.
- **ESPAsyncWebServer.h**: Used for creating asynchronous web servers.
- **AsyncWebSocket.h**: Used for handling WebSocket connections.
- **FS.h**: Provides file system support.
- **SPIFFS.h**: Manages SPIFFS, allowing us to store and read certificates.

```cpp
const char* ssid = "project";           
const char* password = "123456789";     
```
- The **SSID** and **password** variables store the WiFi credentials.

```cpp
AsyncWebServer server(443);             
AsyncWebSocket ws("/ws");               
```
- **server(443)**: Initializes the WebSocket server on port 443 for HTTPS and WSS.
- **ws("/ws")**: Defines the WebSocket endpoint as `/ws`.

```cpp
const char *certPath = "/serverCert.pem";   
const char *privateKeyPath = "/privateKey.pem";
```
- Paths to the SSL certificate and private key stored in SPIFFS.

```cpp
WiFiClientSecure client;
```
- A secure WiFi client is initialized for handling SSL connections.

### `setup()` Function

```cpp
void setup() {
  Serial.begin(115200);               
  WiFi.begin(ssid, password);        
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi Connected");
```
- Connects the ESP32 to the WiFi network.
- Waits until the connection is established.

```cpp
  if (!SPIFFS.begin()) {
    Serial.println("An error occurred while mounting SPIFFS");
    return;
  }
```
- Initializes SPIFFS for storing the SSL certificates.

```cpp
  File cert = SPIFFS.open(certPath, "r");
  File privateKey = SPIFFS.open(privateKeyPath, "r");

  if (!cert || !privateKey) {
    Serial.println("Failed to open cert or key file.");
    return;
  }

  client.loadCertificate(cert);
  client.loadPrivateKey(privateKey);
```
- Loads the SSL certificate and private key from SPIFFS and sets them for the **WiFiClientSecure**.

```cpp
  ws.onEvent(onEvent);                 
  server.addHandler(&ws);               
  server.beginSecure(certPath, privateKeyPath);  
}
```
- Registers the WebSocket event handler.
- Starts the WebSocket server with secure SSL/TLS.

### `loop()` Function

```cpp
void loop() {
  ws.cleanupClients(); 
}
```
- Cleans up inactive WebSocket clients regularly.

### WebSocket Event Handler

```cpp
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("Client %u connected\n", client->id());
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("Client %u disconnected\n", client->id());
  }
}
```
- Handles WebSocket events, such as client connection and disconnection.

---

## Key Changes

1. **WiFiClientSecure**: Used for handling SSL/TLS connections.
2. **Certificates**: SSL certificates and private key are required for establishing a secure connection.
3. **WebSocket Client**: On the client-side, you need to change the WebSocket URL from `ws://` to `wss://`.

## Steps to Follow

1. **Generate SSL Certificates**: Use OpenSSL to generate a self-signed certificate or obtain one from a trusted Certificate Authority.
2. **Upload Certificates to ESP32**: Store the `.pem` files in **SPIFFS**.
3. **Change WebSocket URL**: On the client-side, update the WebSocket connection URL to use `wss://` (e.g., `wss://your-esp32-ip/ws`).

---

This setup ensures encrypted communication between your ESP32 WebSocket server and the WebSocket client using **wss** for secure WebSocket communication.

--- 

Feel free to provide any other sections you'd like added, and I can modify the document further!