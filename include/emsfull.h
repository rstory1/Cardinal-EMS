#ifndef EMSFULL_H
#define EMSFULL_H

#include <QObject>
#include <QGraphicsScene>
#include <QWidget>
#include <QtWidgets>

#include <instruments/rpmindicator.h>
#include <instruments/bargraph.h>
#include <instruments/fuelmanagement.h>
#include <instruments/manifoldpressure.h>
#include <instruments/textBoxGauge.h>
#include <instruments/fueldisplay.h>
#include <instruments/chtegtgauge.h>
#include <instruments/windvector.h>
#include <instruments/hourmeter.h>
#include <gaugesettings.h>

class emsFull : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit emsFull(QObject* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    RpmIndicator rpmIndicator;
    BarGraph oilTemperature;
    BarGraph oilPressure;
    BarGraph voltMeter;
    BarGraph ampereMeter;
    BarGraph fuelFlow;
    TextBox insideAirTemperature;
    TextBox outsideAirTemperature;
    FuelManagement fuelManagement;
    FuelDisplay fuelDisplay;
    ManifoldPressure manifoldPressure;
    ChtEgt chtEgt;
    WindVector windVector;
    HourMeter hobbs;

    QSettings gaugeSettings;
    QSettings settings;

    void setupRpmIndicator();
    void setupBarGraphs();
    void setupExhaustGasTemperature();
    void setupCylinderHeadTemperature();
    void setupFuelManagement();
    void setupManifoldPressure();
    void setupChtEgt();
    void setupWindVector();
    void setupHourMeter();
    void setFuelData(double fuelFlowValue, double fuelAbsoluteValue);

};

#endif // EMSFULL_H
