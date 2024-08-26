#if 0

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const String ssid = "private1";
const String password = "private11";

// Initialize the LCD with I2C address 0x27 and 16 columns by 2 rows
LiquidCrystal_I2C lcd(0x27, 20, 4);


const String questions[] = {
  "Do you have headache",
  "Do you have a fever?",
  "Do you have a cough?",
  "Are you experiencing fatigue?",
  "Are you experiencing any pain or discomfort?",
  "How would you rate your overall energy level?",
  "Have you noticed any changes in your appetite?",
  "Are you able to sleep well?",
  "Do you have difficulty breathing?",
  "Are you experiencing nausea or vomiting?",
  "Are you currently pregnant?"
  
  "Are you currently taking any medications prescribed by your doctor?"
};

// 
const String pre



const int noOfQuestions = 10;



String answers[noOfQuestions];  // Store patient answers

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  displayQuestion();
}

void loop() {
  for( int currentQuestion = 0; currentQuestion <= noOfQuestions; currentQuestion++) {

    // if(isThatAll = true) {
    //   exit loop
    // }
    // logic for selecting yes or no
    bool currentAnswer = true;
    answers[currentQuestion] = currentAnswer;
  }
  delay(100);
}

// void displayQuestion() {
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print(questions[questionIndex]);
// }

/*
void sendDataToServer() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://your_server.com/storePatientData");
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"answers\": [";
    for (int i = 0; i < 3; i++) {
      payload += "\"" + answers[i] + "\"";
      if (i < 2) payload += ", ";
    }
    payload += "]}";

    int httpCode = http.POST(payload);

    if (httpCode > 0) {
      String response = http.getString();
      Serial.println(response);
      // Display prescription or next steps
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Prescription sent");
    }

    http.end();
  }
}
*/

#endif