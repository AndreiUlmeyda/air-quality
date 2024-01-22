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
  airSensor.setAltitudeCompensation(260);
  // approx. avg. pressure in Dresden, Saxony in mBar
  airSensor.setAmbientPressure(1030);
  // estimated temperature difference between the sensor and its
  // surroundings that would be caused by self heating during operation
  airSensor.setTemperatureOffset(1);
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
  display.fillScreen(black);
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

void drawCO2(int co2Concentration)
{
  int COLOR_LOW = green;

  int THRESHOLD_MEDIUM = 1000;
  int COLOR_MEDIUM = yellow;

  int THRESHOLD_HIGH = 2000;
  int COLOR_HIGH = red;

  int drawColor = COLOR_LOW;
  if (co2Concentration >= THRESHOLD_MEDIUM)
    drawColor = COLOR_MEDIUM;
  if (co2Concentration >= THRESHOLD_HIGH)
    drawColor = COLOR_HIGH;

  display.fillRect(0, 0, 96, 48, drawColor);

  display.setTextColor(black);
  display.setCursor(15, 15); // (long side, short side)
  display.setTextSize(3);
  display.print(co2Concentration);
}

void drawTemperature(float temperature)
{
  float tempOffsetToMatchReferenceSensor = -4.9;

  float correctedTemperature = temperature + tempOffsetToMatchReferenceSensor;

  display.fillRect(49, 49, 48, 16, black);
  display.setTextColor(yellow);
  display.setCursor(40, 50); // (long side, short side)
  display.setTextSize(2);
  String firstFourCharacters = String(correctedTemperature).substring(0, 4);
  display.print(firstFourCharacters);
}

void drawHumidity(float humidity)
{
  display.fillRect(0, 49, 34, 16, black);
  display.setTextColor(blue);
  display.setCursor(8, 50); // (long side, short side)
  display.setTextSize(2);
  display.print(int(round(humidity)));
}

void loop()
{
  if (airSensor.dataAvailable())
  {
    int co2 = airSensor.getCO2();
    float temperature = airSensor.getTemperature();
    float humidity = airSensor.getHumidity();

    drawCO2(co2);
    drawTemperature(temperature);
    drawHumidity(humidity);

    char serialOutput[95];
    sprintf(serialOutput,
            "co2=%d,temperature=%s,humidity=%s",
            co2,
            String(temperature),
            String(humidity));
    Serial.println(serialOutput);
  }
}