#include "alarm.h"

alarm::alarm()
{
    wiringPiSetup();
    pinMode(18, OUTPUT);
    pinMode(17, INPUT);
}


void alarm::soundAlarm(int alarmColor, int alarmSeverity, QString alarmText, QString alarmGauge)
{
    digitalWrite(18, HIGH);
    delay (500);
    digitalWrite (18,  LOW);
    delay (500);
}
