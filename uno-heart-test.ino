#include <SPI.h>
#include <Wire.h>

const int sensorPin = A0;                             // A0 is the input pin for the heart rate sensor
int sensorValue;                                        // Variable to store the value coming from the sensor
int count = 0;
unsigned long starttime = 0;
int heartrate = 0;
boolean counted = false;

const int D13 = 13;

void setup (void) {
  Serial.begin (115200);       
  pinMode(D13, OUTPUT);                      
}


void loop () { 
  starttime = millis();
    while (millis()<starttime+20000)                                            // Reading pulse sensor for 20 seconds
    { sensorValue = analogRead(sensorPin);
        Serial.println ("Sensor Value = " + String(sensorValue));
          delay(50);
      if ((sensorValue >= 590 && sensorValue <=680) && counted == false)    // Threshold value is 590 (~ 2.7V)
      {  count++;
        digitalWrite (D13,HIGH);
          delay (10);       
        digitalWrite (D13, LOW);
        counted = true;
        }
      else if (sensorValue < 590)
       {  counted = false;
          digitalWrite (D13, LOW);
          }
      }
    Serial.print ("Pulse ");
    Serial.println (count);
    heartrate = (count)*3;                              // Multiply the count by 3 to get beats per minute
    Serial.println ();
    Serial.print ("BPM = ");
    Serial.println (heartrate);                         // Display BPM in the Serial Monitor
    Serial.println ();
    count = 0;
    
    //delay(1000);
}