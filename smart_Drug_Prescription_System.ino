void beginTheSerialMonitor() {
  Serial.begin(115200);
}

//====================================================================================================================================
/*
  - put temperature sensor in your armpit till it stops changing value
  - then place your thumb on the heart rate sensor till it becomes stable,
  - restart the system to measure again
* System will measure temperature and heart-rate
* System will prescribe drug and further deatails will be sent to user's telegram
* we should have 10 prescription cases
* informations like age, weight, height, and date will be there 
* // hw-827 heart- rate sensor
*
* #define BOT_TOKEN "6555915313:AAGtNf4aADfpbgbe8S3L-83bSeLerVB8LQE"
* add age prompt
* remove malaria
make the precription more formal, give one sentence not heighlighting each problem one by one 
* 
*/
