#ifndef FUELDISPLAY_H
#define FUELDISPLAY_H

#include <QtWidgets>
#include "bargraph.h"

//! FuelDisplay Class
/*!
 * This class creates a compact fuel window to display various calculations related to fuel flow and flight plan information if available.
*/

class FuelDisplay : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit FuelDisplay(QGraphicsObject* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setFuelFlow(qreal value);
    void setTimeToDestination(double time);
    void reduceFuelAmount(double fuel)
    {
        fuelAmount -= fuel;
        this->update();
    }

    bool gpsAvailable = false;
private:
    QSettings settings;
    qreal fuelAmount;
    QString fuelUnits;
    float fuelFlow;
    double rawFuelFLowValue;
    double timeToDestination;
    QRectF remainingFuelRect;
    QRectF remainingFuelAtDestinationRect;
    QRectF mpgRect;
    QRectF rangeRect;
    BarGraph fuelFlowGraph;
    float range;
    float avgFuelFlow;
    int fuelBurnUpdateInterval = 4000;

    QTimer fuelBurnTimer;

    bool dataIsValid = false;

private slots:
    void updateFuelBurn();

public slots:
    void onFuelAmountChange(); // Direction is + or -
    void onInitializeFuelLevel(qreal level) {
        fuelAmount = level;
    };

signals:
    void saveFuelState(qreal level);
    void getInitialFuelLevel();
};

#endif // FUELDISPLAY_H
