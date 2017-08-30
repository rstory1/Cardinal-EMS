#ifndef HOURMETER_H
#define HOURMETER_H

#include <QtWidgets>

class HourMeter : public QObject
{
    Q_OBJECT
public:
    explicit HourMeter(QObject *parent = 0);

private:
//    int sec;
//    int min;
//    int hour;
    struct clock{
        int sec;
        int min;
        int hour;
    };

    clock hobbs;
    clock flight;

public slots:
    void onTic(bool isFlying);
};

#endif // HOURMETER_H
