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
#include <QtGui/QColor>

class sensorConvert : public QThread
{
    Q_OBJECT
public:
    explicit sensorConvert(QObject *parent = 0);

private:
    QString thermocoupleTypeCht;
    QString thermocoupleTypeEgt;
    QString temperatureScale; // K, C, R, or F

    void setThermocoupleTypeCht(QString type); // K or J
    void setThermocoupleTypeEgt(QString type); // K or J
    void convertThermocouple(int resistance);
    void convertEgt(int egt1, int egt2, int egt3, int egt4);
    void convertCht(int cht1, int cht2, int cht3, int cht4);

    void convertFuelFlow(int pulses);

    void convertOilTemp(int voltage);
    void convertOilPress(int voltage);

    void convertRpm(int pulses);

signals:
    void updateEgt(int egt1, int egt2, int egt3, int egt4);
    void updateCht(int cht1, int cht2, int cht3, int cht4);

public slots:
};

#endif // SENSORCONVERT_H
