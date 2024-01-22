#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"
#include <Adafruit_SSD1331.h>

SCD30 airSensor;

// https://digilent.com/reference/pmod/pmodoledrgb/start
#define CS 4    // chip select
#define DC 21   // data/command control
#define MOSI 18 // master-out slave-in
#define SCK 5   // serial clock
#define RES 14  // power reset

Adafruit_SSD1331 display = Adafruit_SSD1331(CS, DC, MOSI, SCK, RES);

#define black 0x0000
#define blue 0x001F
#define red 0xF800
#define green 0x07E0
#define cyan 0x07FF
#define magenta 0xF81F
#define yellow 0xFFE0
#define white 0xFFFF

void calibrateSensor()
{
  // measurement interval in seconds
  airSensor.setMeasurementInterval(2);
  // approx. altitude of Dresden, Saxony in meters
  airSensor.setAltitudeCompensation(112);
  // approx. avg. pressure in Dresden, Saxony in mBar
  airSensor.setAmbientPressure(1020);
  // estimated temperature difference between the sensor and its
  // surroundings that would be caused by self heating during operation
  airSensor.setTemperatureOffset(2);
}

void startSensor()
{
  bool sensorReady = airSensor.begin();
  if (!sensorReady)
  {
    Serial.println("Sensor wiring or setup faulty. Not continuing...");
    while (true)
    {
      ;
    }
  }
}

void setupI2C()
{
  Wire.begin();
}

void waitForSerialConnection()
{
  while (!Serial)
  {
    ;
  }
}

void startSerialConnection()
{
  Serial.begin(9600);
  waitForSerialConnection();
}

void startDisplay()
{
  display.begin();
  display.fillScreen(green);
}

// entry point
void setup()
{
  startSerialConnection();
  setupI2C();
  startSensor();
  calibrateSensor();
  startDisplay();
}

void loop()
{
  if (airSensor.dataAvailable())
  {
    int co2 = airSensor.getCO2();
    String temperature = String(airSensor.getTemperature());
    String humidity = String(airSensor.getHumidity());

    char serialOutput[95];
    sprintf(serialOutput,
            "co2=%d,temperature=%s,humidity=%s",
            co2,
            temperature,
            humidity);
    Serial.println(serialOutput);
  }
}