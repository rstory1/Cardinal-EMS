#include "hourmeter.h"

HourMeter::HourMeter(QObject *parent) : QObject(parent)
{
    hobbs.hour = 0;
    hobbs.min = 0;
    hobbs.sec = 0;

    flight.hour = 0;
    flight.min = 0;
    flight.sec = 0;
}


void HourMeter::onTic(bool isFlying) {
    if (hobbs.sec < 59) {
        hobbs.sec = hobbs.sec + 1;
    } else {
        hobbs.sec = 0;

        if (hobbs.min < 59) {
            hobbs.min = hobbs.min + 1;
        } else {
            hobbs.min = 0;
            hobbs.hour = hobbs.hour + 1;
        }
    }

    if (isFlying) {
        if (flight.sec < 59) {
            flight.sec = flight.sec + 1;
        } else {
            flight.sec = 0;

            if (flight.min < 59) {
                flight.min = flight.min + 1;
            } else {
                hobbs.min = 0;
                flight.hour = flight.hour + 1;
            }
        }
    }

    qDebug() << QString::number(hobbs.hour, 'f', 0).rightJustified(2,'0').append(QString(":").append(QString::number(hobbs.min, 'f',0).rightJustified(2,'0'))).append(QString(":").append(QString::number(hobbs.sec, 'f',0).rightJustified(2,'0')));
}
