#include "alarm.h"

alarm::alarm()
{
    wiringPiSetupSys();
    pinMode(18, OUTPUT);
    pinMode(17, INPUT);

    wiringPiISR(17,INT_EDGE_FALLING, &buttonClicked);
}


void alarm::soundAlarm(int alarmColor, int alarmSeverity, QString alarmText, QString alarmGauge)
{
    digitalWrite(18, HIGH);
}

void alarm::buttonClicked()
{
    digitalWrite(18, LOW);
}
