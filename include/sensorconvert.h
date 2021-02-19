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
#include <QMessageBox>

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

    qreal cht[4];
    qreal rpm, fuelFlow, oilTemp, oilPress, amps, volts, egt1, egt2, egt3, egt4, oat, iat, fuelPress;

    qreal resistance;
    qreal temp;
    qreal puResistorValue = 1500;
    qreal pdResistorValue = 100;

    qreal tempVoltage;
    qreal tempCurrent;
    qreal current1;
    qreal current2;

    qreal tempCHT[4];

    qreal currentAdc;

    qreal manP;

    QString emsSerialString;
    QByteArray emsSerialStringByteArray;

    void setThermocoupleTypeCht(QString type); // K or J
    void setThermocoupleTypeEgt(QString type); // K or J
    void setTemperatureScale(QString scale); // K, C, R, or F
    void convertThermocouple(int resistance);

    void convertEgt(double volt1, double volt2, double volt3, double volt4);
    void convertCht(qreal adc1, qreal adc2, qreal adc3, qreal adc4);

    void convertIat(double sensorValue);

    void convertFuelFlow(double pulses);

    void convertOilTemp(qreal adc);
    void convertOilPress(qreal adc);

    void convertRpm(double pulses);

    qreal convertTemperature(qreal temp, char convertToUnits);

    void setKFactor(qreal kFac);

    void convertCurrent(qreal adc, int sensorNum);

    void convertMAP(qreal adc);

    void convertOAT(qreal adc);

    void convertFuelP(qreal adc);

    qreal filterReading(qreal inputVal, qreal preVal, qreal dt, qreal tau);

    bool popPulses;

    QTimer timerPulses;

    //QElapsedTimer timer;
signals:
    void userMessage(QString,QString,bool);
    void updateMonitor(qreal rpm, qreal fuelFlow, qreal oilTemp, qreal oilPress, qreal amps, qreal amps2, qreal volts, qreal egt1, qreal egt2, qreal egt3, qreal egt4, qreal cht1, qreal cht2, qreal cht3, qreal cht4, qreal oat, qreal iat, qreal map, qreal fuelPress);
    void sendSerialData(QByteArray &data);

public slots:
    void onRdacUpdate(qreal fuelFlow1, qreal fuelFlow2, quint16 tc1, quint16 tc2, quint16 tc3, quint16 tc4, quint16 tc5, quint16 tc6, quint16 tc7, quint16 tc8, qreal oilT, qreal oilP, qreal ax1, qreal ax2, qreal fuelP, qreal coolantT, qreal fuelL1, qreal fuelL2, quint16 rpm1, qreal rpm2, qreal map, qreal curr, quint16 intTemp, qreal volts);
    void onZeroCurrent();

private slots:
    void showPulses();
    void debugSend();
};

#endif // SENSORCONVERT_H
