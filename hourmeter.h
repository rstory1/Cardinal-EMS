#ifndef HOURMETER_H
#define HOURMETER_H

#include <QtWidgets>

class hourMeter : public QGraphicsObject
{
    Q_OBJECT
public:
    hourMeter();

private:
    int sec;
    int min;
    int hour;

public slots:
//    void onTic();
};

#endif // HOURMETER_H
