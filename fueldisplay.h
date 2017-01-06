#ifndef FUELDISPLAY_H
#define FUELDISPLAY_H

#include <QtWidgets>
#include "bargraph.h"

class FuelDisplay : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit FuelDisplay(QGraphicsObject* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setFuelFlow(double value);
    void setTimeToDestination(double time);
    void reduceFuelAmount(double fuel)
    {
        fuelAmount -= fuel;
        this->update();
    }
private:
    QSettings settings;
    double fuelAmount;
    QString fuelUnits;
    double fuelFlow;
    double timeToDestination;
    QRectF remainingFuelRect;
    QRectF remainingFuelAtDestinationRect;
    BarGraph fuelFlowGraph;
};

#endif // FUELDISPLAY_H
