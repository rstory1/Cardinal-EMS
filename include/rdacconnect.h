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
#include <QtGui/QColor>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>

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
    quint16 thermocouple[12];
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

struct RDACmessage1
{
public:
	RDACmessage1();
    quint16 flow1;
    quint16 pulseRatio1;
    quint16 flow2;
    quint16 pulseRatio2;
    quint16 thermocouple[12];
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
};

struct RDACmessage2
{
public:
	RDACmessage2();
	quint16 oilTemperature;
	quint16 oilPressure;
	quint16 fuelLevel1;
	quint16 fuelLevel2;
	quint16 voltage;
	quint16 internalTemperature;
	quint16 cht1;
	quint16 cht2;
	quint16 manifoldPressure;
};

struct RDACmessage3
{
public:
	RDACmessage3();
	quint16 timeBetweenPulses;
};

struct RDACmessage4
{
public:
	RDACmessage4();
	quint16 thermocouple[12];
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
private:
	bool searchStart(QByteArray *data);
	rdacResults checkPatternValidity(QByteArray *data, quint8 &messageType);
    QDateTime lastMessageReception;
	QDateTime lastMessage1;
    void handleMessageRDACXF(QByteArray *data);
	void handleMessage2(QByteArray *data);
	void handleMessage3(QByteArray *data);
	void handleMessage4(QByteArray *data);
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
	void updateDataMessage1(double fuelFlowValue, double fuelAbsoluteValue);
	void updateDataMessage2(double insideAirTemperatureValue, double outsideAirTemperatureValue, double ampereValue, double oilTemperatureValue, double oilPressureValue, double voltageValue, double manifoldPressure);
	void updateDataMessage3(double revolutionsPerMinute);
	void updateDataMessage4egt(quint16 egt1, quint16 egt2, quint16 egt3, quint16 egt4);
	void updateDataMessage4cht(quint16 cht1, quint16 cht2, quint16 cht3, quint16 cht4);
	void userMessage(QString title, QString content, bool endApplication);
	void statusMessage(QString text, QColor color);
    void rdacUpdateMessage(qreal fuelFlow1, qreal fuelFlow2, quint16 tc1, quint16 tc2, quint16 tc3, quint16 tc4, quint16 tc5, quint16 tc6, quint16 tc7, quint16 tc8, qreal oilT, qreal oilP, qreal ax1, qreal ax2, qreal fuelP, qreal coolantT, qreal fuelL1, qreal fuelL2, quint16 rpm1, qreal rpm2, qreal map, qreal curr, quint16 intTemp, qreal volts);
};

#endif // RDACCONNECT_H
