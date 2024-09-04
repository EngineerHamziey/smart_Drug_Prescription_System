#if 0

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pins
const int oneWireBus = 5;
const int heartRatePin = 34; // Assuming analog pin 34 for heart rate sensor

// Wi-Fi and Telegram bot credentials
char ssid[] = "project";
char password[] = "123456789";
WiFiClientSecure client;
UniversalTelegramBot bot("6555915313:AAGtNf4aADfpbgbe8S3L-83bSeLerVB8LQE", client);

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); 

// OneWire setup
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Array to store answers
String answers[12]; // Adjusted size for the 12 questions
int currentQuestion = 0;

const String questions[] = {
  "Do you have a headache?",
  "Do you have a fever?",
  "Do you have a cough?",
  "Are you experiencing fatigue?",
  "Are you experiencing any pain or discomfort?",
  "How would you rate your overall energy level?",
  "Have you noticed any changes in your appetite?",
  "Are you able to sleep well?",
  "Do you have difficulty breathing?",
  "Are you experiencing nausea or vomiting?",
  "Are you currently pregnant?",
  "Are you currently taking any medications prescribed by your doctor?"
};

// Function to display message on the LCD
void displayMessage(String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
}

// Function to send the next question
void sendNextQuestion(String chatId) {
  if (currentQuestion < sizeof(questions) / sizeof(questions[0])) {
    bot.sendMessage(chatId, questions[currentQuestion], "");
  } else {
    bot.sendMessage(chatId, "Thank you for answering the questions. Processing your information...", "");
    // Placeholder for drug recommendation logic
  }
}

// Function to check temperature and heart rate
void checkVitals(String chatId) {
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
  int heartRate = analogRead(heartRatePin); // Adjust logic based on the heart rate sensor library used
  bot.sendMessage(chatId, "Let's start by taking your temperature and heart rate. \nPlace your thumb on the heart rate sensor and the temperature sensor inyour ampit.", "");
  displayMessage("Let us check your vitals");
  
  if (temperature < 36.5 || temperature > 37.5 || heartRate < 60 || heartRate > 100) {
    bot.sendMessage(chatId, "Please see a doctor.", "");
    displayMessage("See a doctor.");
  } else {
    sendNextQuestion(chatId);
  }
}



void connectWifi() {
  WiFi.mode(WIFI_STA);
  WiFiManager myWifiManager;
  Serial.println("Connect your device to SMART HOME wifi to configure settings");

  myWifiManager.setConfigPortalTimeout(60);
  bool wifiConnected = myWifiManager.autoConnect("SMART DRUG PRESCRIPTION", "password");

  if (wifiConnected) {
    Serial.println("Successfully connected to WiFi.");
  } else {
    Serial.println("Failed to connect to WiFi. Restarting system...");
    ESP.restart();
  }
}


void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  displayMessage("Initializing...");

  // Initialize Wi-Fi using WiFiManager
  connectWifi();
  
  displayMessage("WiFi Connected!");

  // Initialize sensors
  sensors.begin();

  // Telegram bot settings
  client.setInsecure();
  bot.longPoll = true;
}

void loop() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  for (int i = 0; i < numNewMessages; i++) {
    String chatId = String(bot.messages[i].chat_id); // Convert chat_id to String
    String text = bot.messages[i].text;

    if (text == "/start") {
      bot.sendMessage(chatId, "Welcome to the Smart Drug Prescription System. Let's start with a few questions.", "");
      currentQuestion = 0;
      checkVitals(chatId);
    } else if (text == "/yes" || text == "/no") {
      answers[currentQuestion] = text;
      currentQuestion++;
      checkVitals(chatId);
    }
  }
}


#endif