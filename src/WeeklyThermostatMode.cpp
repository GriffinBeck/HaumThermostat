#include "ThermostatMode.h"
#include "WeeklyThermostatMode.h"

class WeeklyThermostatMode : ThermostatMode
{

private:
  long lastTimeUpdate = 0;
  unsigned int day;
  unsigned int hour;
  unsigned int lastTargetTemp;
  WeekTemps *weekTemperature;
  unsigned int getTargetTemperature()
  {
    if (millis() - lastTimeUpdate > 60000)
    {
      day = Time.day();
      hour = Time.hour();
      lastTimeUpdate = millis();
    }
    int hold = (*weekTemperature).days[day].hours[hour].targetTemp;
    if (hold == 0)
    {
      return lastTargetTemp;
    }
    else
    {
      lastTargetTemp = hold;
    }
    return lastTargetTemp;
  }

public:
  WeeklyThermostatMode()
  {
    targetTemperature = 75;
  }

  WeeklyThermostatMode(int target)
  {
    targetTemperature = target;
  }

  bool shouldCool(double currTemp, double humidity)
  {
    return currTemp + 2 > getTargetTemperature();
  }

  bool shouldHeat(double currTemp, double humidity)
  {
    return currTemp - 2 < getTargetTemperature();
  }
  bool shouldFan(double currTemp, double humidity)
  {
    return shouldHeat(currTemp, humidity) || shouldCool(currTemp, humidity);
  }

  void update()
  {
    //TODO:Implment loading mechanism for weekly temperature programing
  }
};
