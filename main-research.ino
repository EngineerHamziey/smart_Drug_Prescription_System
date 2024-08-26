#if 1

#include <WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Wi-Fi credentials
String wifiSSID = "project";
String wifiPassword = "123456789";
// Hamzatronics bot token: 1389612925
// Telegram BOT Token
#define BOT_TOKEN "6555915313:AAGtNf4aADfpbgbe8S3L-83bSeLerVB8LQE"

// Replace with your network credentials
WiFiClientSecure securedClient;
UniversalTelegramBot bot(BOT_TOKEN, securedClient);

#define oneWireBus 5 // DS18B20 pin
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

#define heartRateSensorPin 34 // Pin for heart rate sensor (can be any ADC pin on ESP32)

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

int questionIndex = 0;
int answers[12];

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  securedClient.setInsecure();
  sensors.begin();
}

void loop() {
  int messageCount = bot.getUpdates(bot.last_message_received + 1);

  for (int i = 0; i < messageCount; i++) {
    String chatID = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    if (text == "/start") {
      bot.sendMessage(chatID, "Welcome to the automatic drug prescription system.");
      askNextQuestion(chatID);
    } else if (text == "/yes" || text == "/no") {
      handleAnswer(chatID, text);
    }
  }
}

void askNextQuestion(String chatID) {
  if (questionIndex < sizeof(questions) / sizeof(questions[0])) {
    bot.sendMessage(chatID, questions[questionIndex]);
    bot.sendMessage(chatID, "Reply with /yes or /no");
  } else {
    evaluateAnswers(chatID);
  }
}

void handleAnswer(String chatID, String answer) {
  answers[questionIndex] = (answer == "/yes") ? 1 : 0;
  questionIndex++;
  askNextQuestion(chatID);
}

void evaluateAnswers(String chatID) {
  // Placeholder for drug recommendation logic
  bot.sendMessage(chatID, "Analyzing your responses...");

  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);

  int heartRate = analogRead(heartRateSensorPin); // Basic analog read for heart rate

  // Placeholder logic for temperature and heart rate analysis
  if (temperature < 36.1 || temperature > 37.2 || heartRate < 60 || heartRate > 100) {
    bot.sendMessage(chatID, "Your vital signs are abnormal. Please consult a doctor.");
  } else {
    bot.sendMessage(chatID, "Your vital signs are normal. Placeholder for drug recommendation.");
  }

  // Reset for next session
  questionIndex = 0;
}


#endif
