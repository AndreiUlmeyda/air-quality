#include <Arduino.h>
#include <Secrets.hpp>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"
  
SCD30 airSensor;

void calibrateSensor(){
  // measurement interval in seconds
  airSensor.setMeasurementInterval(4);
  // approx. altitude of Dresden, Saxony in meters
  airSensor.setAltitudeCompensation(112); 
  // approx. avg. pressure in Dresden, Saxony in mBar
  airSensor.setAmbientPressure(1020); 
  // estimated temperature difference between the sensor and its
  // surroundings that would be caused by self heating during operation 
  airSensor.setTemperatureOffset(2);
}

void startSensor(){
  bool sensorReady = airSensor.begin();
  if (!sensorReady){
    Serial.println("Sensor wiring or setup faulty. Not continuing...");
    while (true) {;}
  }
}

void setupI2C(){
  Wire.begin();
}

void waitForWifiConnection(){
  Serial.print("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED){
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

// entry point
void setup() {
  startSerialConnection();
  startWifiConnection();
  setupI2C();

  startSensor();
  calibrateSensor();
}

void loop() {
}