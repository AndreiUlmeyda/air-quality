#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h"

SCD30 airSensor;

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

// entry point
void setup()
{
  startSerialConnection();
  setupI2C();
  startSensor();
  calibrateSensor();
}

void loop()
{
  delay(1000);
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