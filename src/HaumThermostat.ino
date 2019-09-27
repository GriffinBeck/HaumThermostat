/*
 * Project HaumThermostat
 * Description: A Smart Thermostat with expandability and soon to many options
 * Author: Griffin Beck
 * Date: July 2019
 */
//#include "TemperatureManager.cpp"

//Imports and Definitions for Climate Control Manager
#define CLIMATE_MODE_COOL 1 //Only allows system to be off or cooling
#define CLIMATE_MODE_HEAT 2 //Only allows system to be off or heating
#define CLIMATE_MODE_AUTO 3 //Allows system to heat, cool, or rest as nessary
#define CLIMATE_MODE_OFF 0  //Disables heating and cooling
#define FAN_MODE_OFF 0
#define FAN_MODE_ON 1
#define FAN_MODE_AUTO 2 //Fan turns on when climate system is running
#include "Adafruit_DHT_Particle.h"
#include "ThermostatMode.cpp"
static long minReadWaitTime = 10000; //For climate sensor

//Climate Vars
double curTemp;
double curHumid;
long timeOfLastRead = -(minReadWaitTime); //set to negative of min read time to allow reading on boot
int climatePIN = D4;
DHT dht = DHT(climatePIN, DHT22);

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
  //Setup Climate Sensor and Climate Control Relays
  tempsetup(D4, D5, D6, D7);

  //Button Initializations
  pinMode(upArrow, INPUT_PULLUP);
  pinMode(downArrow, INPUT_PULLUP);
  pinMode(menuButton, INPUT_PULLUP);
  attachInterrupt(upArrow, buttonUp, CHANGE);
  attachInterrupt(downArrow, buttonDown, CHANGE);
  attachInterrupt(menuButton, buttonMenu, CHANGE);
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  runClimateCheck();
  // The core of your code will likely live here.
}

//User Input handling
//TODO Implement Debouncer for buttons
boolean buttonDebouncer(long lastTime)
{
  return millis() - lastTime > 50;
}

long buttonUpLastPressTime = 0;
boolean buttonUpLastState = false;
void buttonUp()
{
  if (buttonDebouncer(buttonDebouncer(buttonUpLastPressTime)))
  {
    if (digitalRead(upArrow) == LOW) //Reading when going low in order to prepare for future handling of short vs long button presses
    {
      upTemperature();
    }
  }
}

long buttonDownLastPressTime = 0;
boolean buttonDownLastState = false;
void buttonDown()
{
  if (buttonDebouncer(buttonDebouncer(buttonDownLastPressTime)))
  {
    if (digitalRead(downArrow) == LOW)
    {
      downTemperature();
    }
  }
}

long buttonMenuLastPressTime = 0;
boolean buttonMenuLastState = false;
void buttonMenu()
{
  //TODO
}

//Prepares climate sensor and all pins for relay controls
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

//Checks of the Thermostat mode allows for cooling
bool canCool()
{
  return climateMode == CLIMATE_MODE_COOL || climateMode == CLIMATE_MODE_AUTO;
}

//Checks if the Thermostat mode allows for heating
bool canHeat()
{
  return climateMode == CLIMATE_MODE_HEAT || climateMode == CLIMATE_MODE_AUTO;
}

//Checks if fan mode allows it to run
bool canFan()
{
  return fanMode == FAN_MODE_AUTO || fanMode == FAN_MODE_ON;
}

//Updates Temperature and Humidity
void getCurClimate()
{
  if (canReadClimate())
  {
    curTemp = dht.getTempFarenheit();
    curHumid = dht.getHumidity();
  }
}

//Checks if climate sensor can be read from
bool canReadClimate()
{
  return millis() - timeOfLastRead > minReadWaitTime;
}

//updates Temperature and checks subclasses for approprate control of climate system
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