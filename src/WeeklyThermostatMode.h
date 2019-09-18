#include "ThermostatMode.h"
#include "DayByHour.h"

typedef struct WeekTemps
{
    struct DayByHour *days; //Array of length 7
};