#if 0

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   // Universal Telegram Bot Library written by Brian Lough: https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "home";
const char* password = "2222222222";

#define BOTtoken "5217809555:AAFi6T94mZSK2pc9fqgs0iQ0zxNlRvT0i90" //Smart meter bot token

#define CHAT_ID "1389612925"//EngineerHamziey chat ID

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

const int relayPin = 17;

String showData() {
  return "Data Shown";
}

// Handle what happens when you receive new messages
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Sorry, you're unauthorized to use this bot.", "");
      continue;
    }
    
    // Print the received message
    String receivedCommand = bot.messages[i].text, replyMsg = "";
    Serial.println(receivedCommand);

    String from_name = bot.messages[i].from_name;
    String commandList = "/showData to view the current Energy comsumption(WH), power comsumption, voltage and current \n";
        commandList += "/loadOn to turn on the load \n";
        commandList += "/loadOff to turn off the load \n";
        commandList += "/loadState to check whether the load is on or off \n";
    
    if (receivedCommand == "/start") {
      String welcomeMsg = "Hello, Mr/Mrs " + from_name + ", this is SMART POWER METER.\n";
      welcomeMsg += "Use the following commands to communicate with the device: \n\n";
      welcomeMsg += commandList;
      replyMsg = welcomeMsg;
    }else if (receivedCommand == "/showData") {
      replyMsg = showData();
    }else if (receivedCommand == "/loadOn") {
      replyMsg = "Load is now on";
      digitalWrite(relayPin, HIGH);
    }else if (receivedCommand == "/loadOff") {
      replyMsg = "LED is now OFF";
      digitalWrite(relayPin, LOW);
    }else if (receivedCommand == "/loadState") {
      if (digitalRead(relayPin)){
        replyMsg = "Load is currently on";
      }
      else{
        replyMsg = "Load is currently off";
      }
    }else {
      replyMsg = "Wrong command entered, kindly check the command list below:";
      replyMsg += commandList;
    }

    bot.sendMessage(chat_id, replyMsg, "");
  }
}

void telegramInitialization() {
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void allInitialization() {
  telegramInitialization();
  beginTheSerialMonitor();

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void setup() {
  allInitialization();
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}

#endif