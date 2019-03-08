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
#include "alarmBox.h"

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
    AlarmBox alarmWindow;

    QSettings gaugeSettings;
    QSettings settings;

    int timeOilTAboveWarmup=0;
    int timeOilTBelowWarmup=0;
    int warmupTemp=0;

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
    void connectSignals();

    QTimer flashTimer;
    QTimer clockTimer;

    public slots:
        void setEngineConds();
        void onAckAlarm() {emit ackAlarm();}
        void onEngineValuesUpdate(qreal rpm, qreal fuelFlow, qreal oilTemp, qreal oilPress, qreal amps, qreal volts, qreal egt1, qreal egt2, qreal egt3, qreal egt4, qreal cht1, qreal cht2, qreal cht3, qreal cht4, qreal oat, qreal iat, qreal map);

    private slots:
        void onAlarmFlash() {emit alarmFlashing();}

    signals:
        void alarmFlashing();
        void ackAlarm();

};

#endif // EMSFULL_H
