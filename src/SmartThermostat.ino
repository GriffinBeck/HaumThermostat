/*
 * Project SmartThermostat
 * Description: A Smart Thermostat with expandability and soon to many options
 * Author: Griffin Beck
 * Date: July 2019
 */
//#include "TemperatureManager.cpp"
#define CLIMATE_MODE_COOL 1
#define CLIMATE_MODE_HEAT 2
#define CLIMATE_MODE_AUTO 3
#define CLIMATE_MODE_OFF 0
#define FAN_MODE_OFF 0
#define FAN_MODE_ON 1
#define FAN_MODE_AUTO 2
#include "Adafruit_DHT_Particle.h"
#include "ThermostatMode.cpp"
static long minReadWaitTime = 10000;

//Climate Vars
double curTemp;
double curHumid;
long timeOfLastRead = -(minReadWaitTime);
int climatePIN;
DHT dht = DHT(D4, DHT22);

int coolPin;
int heatPin;
int fanPin;
int climateMode = CLIMATE_MODE_AUTO;
int fanMode = FAN_MODE_AUTO;

bool cooling = false;
bool heating = false;
bool faning = false;

ThermostatMode thermostat;

//Interface Vars
int upArrow = A0;
int downArrow = A1;
int menuButton = D0;
// setup() runs once, when the device is first turned on.
void setup()
{
  Serial.begin(9600);
  tempsetup(D4, D5, D6, D7);
  // Put initialization like pinMode and begin functions here.
  pinMode(upArrow, INPUT);
  pinMode(downArrow, INPUT);
  pinMode(menuButton, INPUT);
  attachInterrupt(upArrow, buttonUp, RISING);
  attachInterrupt(downArrow, buttonDown, RISING);
  attachInterrupt(menuButton, buttonMenu, RISING);
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  runClimateCheck();
  // The core of your code will likely live here.
}

void buttonUp()
{
  upTemperature();
}

void buttonDown()
{
  downTemperature();
}

void buttonMenu()
{
  //TODO
}

void tempsetup(int climatePIN, int coolPIN, int heatPIN, int fanPIN)
{
  pinMode(climatePIN, INPUT);
  //dht = DHT(climatePIN, DHT22, 6);
  dht.begin();
  thermostat.setTargetTemp(75);

  coolPin = coolPIN;
  heatPin = heatPIN;
  fanPin = fanPIN;

  pinMode(coolPin, OUTPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
}

bool canCool()
{
  return climateMode == CLIMATE_MODE_COOL || climateMode == CLIMATE_MODE_AUTO;
}

bool canHeat()
{
  return climateMode == CLIMATE_MODE_HEAT || climateMode == CLIMATE_MODE_AUTO;
}

bool canFan()
{
  return fanMode == FAN_MODE_AUTO || fanMode == FAN_MODE_ON;
}

void getCurClimate()
{
  if (canReadClimate())
  {
    curTemp = dht.getTempFarenheit();
    curHumid = dht.getHumidity();
  }
}

bool canReadClimate()
{
  return millis() - timeOfLastRead > minReadWaitTime;
}

void runClimateCheck()
{
  getCurClimate();

  cooling = false;
  if (canCool())
  {
    cooling = thermostat.shouldCool(curTemp, curHumid);
  }
  heating = false;
  if (canHeat())
  {
    heating = (thermostat).shouldHeat(curTemp, curHumid);
  }
  faning = false;
  if (canFan())
  {
    faning = (thermostat).shouldFan(curTemp, curHumid) || fanMode == FAN_MODE_ON;
  }

  if (cooling)
  {
    digitalWrite(coolPin, HIGH);
    digitalWrite(heatPin, LOW);
  }
  else if (heating)
  {
    digitalWrite(coolPin, LOW);
    digitalWrite(heatPin, HIGH);
  }
  else
  {
    digitalWrite(coolPin, LOW);
    digitalWrite(heatPin, LOW);
  }

  if (faning)
  {
    digitalWrite(fanPin, HIGH);
  }
  else
  {
    digitalWrite(fanPin, LOW);
  }
  Serial.println(":");
  Serial.println(thermostat.getTargetTemp());
  Serial.println(curTemp);
}

void upTemperature()
{
  thermostat.setTargetTemp(thermostat.getTargetTemp() + 1);
  //TODO Adjust Display  Accordingly
}

void downTemperature()
{
  thermostat.setTargetTemp(thermostat.getTargetTemp() - 1);
  //TODO Adjust Display Accordingly
}