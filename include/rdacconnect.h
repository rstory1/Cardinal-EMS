//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2012 Tobias Rad                                        //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef RDACCONNECT_H
#define RDACCONNECT_H

#include <QtCore>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <sensorconvert.h>

//! RDAC Connect Class
/*!
 * This class interprets the messages coming from the MGL RDAC.
*/

#pragma pack(1)
struct RDACXFmessage
{
public:
    RDACXFmessage();
    char dle;
    char stx;
    char messageId;
    char messageVer;
    quint16 flow1;
    quint16 pulseRatio1;
    quint16 flow2;
    quint16 pulseRatio2;
    quint16 thermocouple1;
    quint16 thermocouple2;
    quint16 thermocouple3;
    quint16 thermocouple4;
    quint16 thermocouple5;
    quint16 thermocouple6;
    quint16 thermocouple7;
    quint16 thermocouple8;
    quint16 thermocouple9;
    quint16 thermocouple10;
    quint16 thermocouple11;
    quint16 thermocouple12;
    quint16 oilTemp;
    quint16 oilPress;
    quint16 aux1;
    quint16 aux2;
    quint16 fuelPress;
    quint16 coolant;
    quint16 fuelLevel1;
    quint16 fuelLevel2;
    quint16 rpm1;
    quint16 rpm2;
    quint16 map;
    quint16 current;
    quint16 internalTemp;
    quint16 volts;
    char checkLow;
    char checkHigh;
};

#pragma pack()

class RDACconnect : public QObject
{
    Q_OBJECT
public:
    RDACconnect(QObject *parent = 0);
    static quint8 calculateChecksum1(QByteArray data);
    static quint8 calculateChecksum2(QByteArray data);
    enum rdacResults {
        rdacResultMessageComplete,
        rdacResultMessageIncomplete,
        rdacResultMessageInvalidChecksum1,
        rdacResultMessageInvalidChecksum2,
        rdacResultMessageIllegalDatatype
    };
    SensorConvert sensorConvert;

private:
    bool searchStart(QByteArray *data);
    rdacResults checkPatternValidity(QByteArray *data, quint8 &messageType);
    QDateTime lastMessageReception;
    QDateTime lastMessage1;
    void handleMessageRDACXF(QByteArray *data);
    QSettings settings;
    QSerialPort *serial;
    QByteArray data;
    float numTries = 0.0;
    float numSuccess = 0.0;
    qreal volts;
    qreal fuelFlow1;
    qreal fuelFlow2;

public slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void rdacUpdateMessage(qreal fuelFlow1, qreal fuelFlow2, quint16 tc1, quint16 tc2, quint16 tc3, quint16 tc4, quint16 tc5, quint16 tc6, quint16 tc7, quint16 tc8, qreal oilT, qreal oilP, qreal ax1, qreal ax2, qreal fuelP, qreal coolantT, qreal fuelL1, qreal fuelL2, quint16 rpm1, qreal rpm2, qreal map, qreal curr, quint16 intTemp, qreal volts, QDateTime lastMessage1);
};

#endif // RDACCONNECT_H
