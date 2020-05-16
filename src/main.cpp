#include <Arduino.h>
void waitForSerialConnection() {
  while (!Serial) {;}
}

void startSerialConnection() {
  Serial.begin(9600);
  waitForSerialConnection();
}

void setup() {
  startSerialConnection();
}

void loop() {
}