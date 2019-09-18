class ThermostatMode
{
private:
public:
  int targetTemperature;
  ThermostatMode();
  ThermostatMode(int targetTemperature);
  bool shouldCool(double currTemp, double humidity);
  bool shouldHeat(double currTemp, double humidity);
  bool shouldFan(double currTemp, double humidity);
};
