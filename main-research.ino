#if 0

#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Initialize the LCD with I2C address 0x27 and 16 columns by 2 rows
LiquidCrystal_I2C lcd(0x27, 20, 4);


const char* questions[] = {
  "Do you have a fever?",
  "Do you have a cough?",
  "Are you experiencing fatigue?",
  "Are you experiencing any pain or discomfort?",
  "How would you rate your overall energy level?",
  "Have you noticed any changes in your appetite?",
  "Are you able to sleep well?",
  "Do you have difficulty breathing?",
  "Are you coughing up phlegm? If yes, what color is it?",
  "Do you feel tightness or pain in your chest?",
  "Are you experiencing nausea or vomiting?",
  "Have you had any changes in bowel movements?",
  "Are you experiencing abdominal pain or discomfort?",
  "Are you experiencing headaches? If yes, how severe are they?",
  "Have you noticed any changes in your vision?",
  "Are you experiencing dizziness or lightheadedness?",
  "Are you experiencing any joint pain or stiffness?",
  "Do you have any muscle pain or weakness?",
  "Have you had any recent injuries or falls?",
  "Are you experiencing any rashes or itching?",
  "Have you noticed any changes in the appearance of your skin (e.g., color changes)?",
  "Are there any wounds or sores that are not healing properly?",
  "Do you have any difficulty urinating?",
  "Are you experiencing pain or burning during urination?",
  "Have you noticed any changes in the frequency or volume of urine?",
  "Have you been feeling anxious or depressed?",
  "Do you have trouble concentrating or making decisions?",
  "Have there been any recent stressful events in your life?",
  "Are you currently taking any medications? If yes, please list them.",
  "Do you have any known allergies to medications or substances?",
  "Have you traveled recently? If yes, where and when?",
  "Have you been in close contact with anyone who is ill?",
  "Do you work or live in an environment that may expose you to specific health risks?"
};

const int noOfQuestions = typeof(questions[]) / typeof(questions[0]);

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
  // Replace this with your actual input handling
  // Simulating patient response with serial input
  if (Serial.available()) {
    char response = Serial.read();
    if (response == 'y' || response == 'n') {
      answers[questionIndex] = (response == 'y') ? "Yes" : "No";
      questionIndex++;
      if (questionIndex < 3) {
        displayQuestion();
      } else {
        sendDataToServer();
      }
    }
  }
  delay(100);
}

void displayQuestion() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(questions[questionIndex]);
}

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


#endif
