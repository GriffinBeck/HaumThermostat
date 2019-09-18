class ThermostatMode
{
  int targetTemperature;

public:
  ThermostatMode() { targetTemperature = 75; }

  ThermostatMode(int target) { targetTemperature = target; }

  bool shouldCool(double currTemp, double humidity)
  {
    return (targetTemperature + 3) < currTemp;
  }

  bool shouldHeat(double currTemp, double humidity)
  {
    return (targetTemperature - 3) > currTemp;
  }

  bool shouldFan(double currTemp, double humidity)
  {
    return shouldHeat(currTemp, humidity) || shouldCool(currTemp, humidity);
  }

  void setTargetTemp(int target)
  {
    if (target > 95 || target < 50)
      return;
    targetTemperature = target;
  }

  int getTargetTemp() { return targetTemperature; }
};
