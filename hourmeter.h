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

    QSettings settings;

    QFont font;

signals:
    void hobbsChange(float hour, float min, float sec);

public slots:
    void onTic(/*bool isFlying*/);
};

#endif // HOURMETER_H
