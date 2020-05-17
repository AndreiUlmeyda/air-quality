#include <Arduino.h>
#include <Config.hpp>
#include <Secrets.hpp>
#include <ESP8266WiFi.h>
#include <ArduinoHttpClient.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"
  
SCD30 airSensor;

void buildAndSendHttpRequestWithMeasurement(HttpClient *httpClient, int co2, int temperature, int humidity) {
  String path = "/write?db=" + String(databaseName);
  String contentType = "application/json";
  String fieldSeparator = String(",");
  String content = "air-quality co2=" + String(co2) + fieldSeparator;
  content = content + "temperature=" + String(temperature) + fieldSeparator;
  content = content + "humidity=" + String(humidity);

  httpClient->beginRequest();

  httpClient->post(path);
  httpClient->sendHeader("Content-Type", contentType);
  httpClient->sendHeader("Content-Length", content.length());
  httpClient->beginBody();
  httpClient->print(content);

  httpClient->endRequest();
}

void printResponse(HttpClient *httpClient) {
   int statusCode = httpClient->responseStatusCode();
  String responseBody = httpClient->responseBody();

  Serial.println(statusCode);
  Serial.println(responseBody);
}

void sendSensorDataToServer(int co2, int temperature, int humidity){
  WiFiClient wifi;
  HttpClient httpClient = HttpClient(wifi, databaseHost, databasePort);
  buildAndSendHttpRequestWithMeasurement(&httpClient, co2, temperature, humidity);
  printResponse(&httpClient);
}

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
  if (airSensor.dataAvailable())
  {
    int co2 = airSensor.getCO2();
    int temperature = airSensor.getTemperature();
    int humidity = airSensor.getHumidity();
    sendSensorDataToServer(co2, temperature, humidity);
  }
}