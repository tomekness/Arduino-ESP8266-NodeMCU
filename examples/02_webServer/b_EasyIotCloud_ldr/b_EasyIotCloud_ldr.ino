/*
 *  Button Input - local Server - simple example
 *  ---------------------------------------------------
 *
 * Using onboard button input (D3 / GPIO 0) sending a button pressed to EasyIotCloud https://easyiot-cloud.com/ 
 *
 * for NodeMCU ESP8266 | autumn 2019 | tomek n. (https://cirg.io) for ixd course
 * based on the example by Igor Jarc http://iot-playground.com
 * 
 * (tested with Arduino IDE 1.8.10)
 *
 */

#include <ESP8266WiFi.h>
#include "EIoTCloudRestApiV1.0.h"
#include <EEPROM.h>

EIoTCloudRestApi eiotcloud;

int ledPin = LED_BUILTIN; // GPIO16 --> can also declared 16 or D0 // for debuging
int ldrPin = A0;  // light sensor Pin (ADC / A0)

// your local-network
const char* ssid = "cirg";
const char* password = "workshop";

// your IoTCloud token 
String TOKEN = "ClW7dpysH5lFT3DAoij0kQk9dJ1SX6KA1vEwdr6c"; 

// your IotCloud moduel ID
String moduleId = "4";
String parameterId = "";  // parameters will be read-in from web

int reportInterval = 6;  // delay(1000 * 10 * REPORT_INTERVAL); // sending every miunte


void setup() {
   
  Serial.begin(115200);     // start serial connection // for debuging 
  Serial.println("Start...");

  // setup the LED  // for debugging
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // turn led on  // my board has an inverted LED pin : /

  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  // using the eiotcloud Library 
  eiotcloud.begin(ssid, password);
  eiotcloud.SetToken(TOKEN);


  // get your parameterID
  parameterId = eiotcloud.GetModuleParameterByName(moduleId, "Sensor.Parameter1");
  Serial.print("parameterId: ");
  Serial.println(parameterId);

  digitalWrite(ledPin, HIGH); // turn led on  // my board has an inverted LED pin : /

}


void loop() {
  
  int sensorValue = analogRead(ldrPin);
  
  Serial.print("LUX = ");
  Serial.print(sensorValue);

  sendValue(sensorValue);

  Serial.println();

  delay(1000 * 10 * reportInterval);
}



void sendValue(int _value){

  bool valueRet = eiotcloud.SetParameterValue(parameterId, String(_value));
  Serial.print(" -- sending value: ");
  Serial.print(_value);
  if(valueRet == 1){
    Serial.print(" -- sending succeded");
  } else {
    Serial.print(" -- sending failed");
  }
}
