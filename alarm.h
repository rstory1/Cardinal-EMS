#ifndef ALARM_H
#define ALARM_H

#include <QString>
#include <wiringPi.h>

class alarm
{
public:
    alarm();
    void soundAlarm(int alarmColor, int alarmSeverity, QString alarmText, QString alarmGauge);
    void clearRpmAlarm();
};

#endif // ALARM_H
