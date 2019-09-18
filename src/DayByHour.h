typedef struct DayByHour
{
    struct HourSetting *hours; //Array of length 24
};

typedef struct HourSetting
{
    unsigned short hourOfDay;
    unsigned int targetTemp;
} HourSetting;