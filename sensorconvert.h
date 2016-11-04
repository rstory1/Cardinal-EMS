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

class SensorConvert : public QThread
{
    Q_OBJECT
public:
    explicit SensorConvert(QObject *parent = 0);

private:
    QString thermocoupleTypeCht;
    QString thermocoupleTypeEgt;
    QString temperatureScale;

    quint16 kFactor;

    void setThermocoupleTypeCht(QString type); // K or J
    void setThermocoupleTypeEgt(QString type); // K or J
    void setTemperatureScale(QString scale); // K, C, R, or F
    void convertThermocouple(int resistance);
    void convertEgt(int volt1, int volt2, int volt3, int volt4);
    void convertCht(int volt1, int volt2, int volt3, int volt4);

    double convertFuelFlow(quint16 pulses);

    double convertOilTemp(int resistance);
    double convertOilPress(int voltage);

    double convertRpm(quint16 pulses);

    quint16 convertTemperature(quint16 temp);

signals:
    void updateEgt(quint16 egt1,quint16 egt2,quint16 egt3,quint16 egt4);
    void updateCht(quint16 cht1,quint16 cht2,quint16 cht3,quint16 cht4);
    void userMessage(QString,QString,bool);
    void updateFuelData(double,double);
    void updateOilData(double,double);
    void updateRpm(double);
    void updateOatIat(double,double);
    void updateAmpsVoltage(double,double);
    void statusMessage(QString,QColor);

public slots:
    void processData(QString data);
};

#endif // SENSORCONVERT_H
