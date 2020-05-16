#include <Arduino.h>
#include <Secrets.hpp>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"
  
SCD30 airSensor;

void startSensor(){
  bool sensorReady = airSensor.begin();
  if (!sensorReady)
  {
    Serial.println("Sensor wiring/setup faulty. Not continuing...");
    while (1)
      ;
  }

}

void setupI2C(){
  Wire.begin();
}

void waitForWifiConnection(){
  Serial.print("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void startWifiConnection(){
  WiFi.begin(SSID, PWD);
  waitForWifiConnection();
}

void waitForSerialConnection() {
  while (!Serial) {;}
}

void startSerialConnection() {
  Serial.begin(9600);
  waitForSerialConnection();
}

void setup() {
  startSerialConnection();
  startWifiConnection();
  setupI2C();

  startSensor();
}

void loop() {
}