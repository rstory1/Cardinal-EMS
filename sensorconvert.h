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
    QString thermocoupleTypeCht;
    QString thermocoupleTypeEgt;
    QString temperatureScale;

    quint16 kFactor;

    double rpm, fuelFlow, oilTemp, oilPress, amps, volts, egt1, egt2, egt3, egt4, cht1, cht2, cht3, cht4, oat, iat;

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

signals:
    void userMessage(QString,QString,bool);
    void updateMonitor(quint16 rpm, quint16 fuelFlow, quint16 oilTemp, quint16 oilPress, quint16 amps, quint16 volts, quint16 egt1, quint16 egt2, quint16 egt3, quint16 egt4, quint16 cht1, quint16 cht2, quint16 cht3, quint16 cht4, quint16 oat, quint16 iat);

public slots:
    void processData(QString data);
};

#endif // SENSORCONVERT_H
