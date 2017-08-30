//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EMS, a graphical gauge to monitor an aircraft's engine               //
// Copyright (C) 2016 Ryan Story                                        //
//                                                                      //
// We are receiving values from our data input stream and then          //
//    converting them to usable values to be sent to the GUI            //
//////////////////////////////////////////////////////////////////////////

#ifndef SENSORCONVERT_H
#define SENSORCONVERT_H

#include <QtCore>
#include <math.h>

//! Sensor Convert Class
/*!
 * This class converts raw sensor values to meaningful numbers.
*/

class SensorConvert : public QThread
{
    Q_OBJECT
public:
    explicit SensorConvert(QObject *parent = 0);

private:
    QSettings settings;
    QSettings gaugeSettings;
    QString thermocoupleTypeCht;
    QString thermocoupleTypeEgt;
    QString temperatureScale;

    qreal kFactor;

    qreal rpm, fuelFlow, oilTemp, oilPress, amps, volts, egt1, egt2, egt3, egt4, cht1, cht2, cht3, cht4, oat, iat;

    void setThermocoupleTypeCht(QString type); // K or J
    void setThermocoupleTypeEgt(QString type); // K or J
    void setTemperatureScale(QString scale); // K, C, R, or F
    void convertThermocouple(int resistance);

    void convertEgt(double volt1, double volt2, double volt3, double volt4);
    void convertCht(double volt1, double volt2, double volt3, double volt4);

    void convertIat(double sensorValue);

    void convertOat(double sensorValue);

    void convertFuelFlow(double pulses);

    void convertOilTemp(double resistance);
    void convertOilPress(double voltage);

    void convertRpm(double pulses);

    double convertTemperature(double temp);

    void setKFactor(qreal kFac);

signals:
    void userMessage(QString,QString,bool);
    void updateMonitor(qreal rpm, qreal fuelFlow, qreal oilTemp, qreal oilPress, qreal amps, qreal volts, qreal egt1, qreal egt2, qreal egt3, qreal egt4, qreal cht1, qreal cht2, qreal cht3, qreal cht4, qreal oat, qreal iat);

public slots:
    void processData(QString data);
    void onRdacUpdate(qreal fuelFlowPulses, qreal volts);
};

#endif // SENSORCONVERT_H
