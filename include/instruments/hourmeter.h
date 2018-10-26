#ifndef HOURMETER_H
#define HOURMETER_H

#include <QtWidgets>

class HourMeter : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit HourMeter(QGraphicsObject* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QString getFlightTime();
    QString getHobbsTime();

    void setEngineOn(bool state);

private:
    struct clock{
        int sec;
        int min;
        int hour;
    };

    clock hobbs;
    clock flight;
    clock engine;

    QString hobbsString;
    QString flightString;

    QSettings settings;

    QFont font;

    bool engineState;

signals:
    void hobbsChange(float hour, float min, float sec);

public slots:
    void onTic(/*bool isFlying*/);
    void onShutdown();
};

#endif // HOURMETER_H
