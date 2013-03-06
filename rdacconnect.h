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
#include <windows.h>

#pragma pack(1)
struct RDACmessage1
{
public:
	RDACmessage1();
	quint16 pulses;
	quint16 timing;
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

#pragma pack()

class RDACconnect : public QThread
{
	Q_OBJECT
public:
	void run();
	static quint8 calculateChecksum1(QByteArray data);
	static quint8 calculateChecksum2(QByteArray data);
private:
	bool searchStart(QByteArray *data);
	bool checkPatternValidity(QByteArray *data, quint8 &messageType);
	QDateTime lastMessage1;
	void handleMessage1(QByteArray *data);
	void handleMessage2(QByteArray *data);
signals:
	void updateDataMessage1(double fuelFlowValue);
	void updateDataMessage2(double oilTemperatureValue, double oilPressureValue, double voltageValue);
};

#endif // RDACCONNECT_H
