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

#include "rdacconnect.h"

RDACmessage1::RDACmessage1() : pulses(0), timing(0)
{
}

RDACmessage2::RDACmessage2() : oilTemperature(0)
  , oilPressure(0)
  , fuelLevel1(0)
  , fuelLevel2(0)
  , voltage(0)
  , internalTemperature(0)
  , cht1(0)
  , cht2(0)
  , manifoldPressure(0)
{
}

RDACmessage3::RDACmessage3() : timeBetweenPulses(0)
{
}

RDACmessage4::RDACmessage4()
{
}

RDACconnect::RDACconnect(QObject *parent) : QThread(parent)
  , settings("./settings.ini", QSettings::IniFormat, parent)
{
}

void RDACconnect::run()
{
	QString portString = "\\\\.\\";
	portString.append(settings.value("Communication/Port", "COM1").toString());
	wchar_t portArray[portString.length() + 1];
	portString.toWCharArray(portArray);
	portArray[portString.length()] = '\0';
	portString.replace("\\\\.\\", "");

	HANDLE serialhCom = CreateFile(portArray, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, NULL);
	if(serialhCom != INVALID_HANDLE_VALUE)
	{
		qDebug() << "Succesful opening" << portString;
	}
	else
	{
		qDebug() << "Could not open" << portString;
		emit userMessage("COM error", "Unable to open " + portString + '\n' + "Settings file: " + settings.fileName() + '\n' + "Closing Application", true);
		exec();
	}

	QByteArray data;
	bool startPatternFound = false;
	forever
	{
		quint8 byte;
		DWORD nrBytes = 0;
		if(ReadFile(serialhCom, &byte, 1, &nrBytes, NULL))
		{
			if(nrBytes == 1)
			{
				data.append(byte);
			}
		}
		else
		{
			emit userMessage("RDAC COM error", "Error reading data, closing application", true);
			exec();
		}

		if(searchStart(&data))
		{
			startPatternFound = true;
			quint8 messageType = 0x00;
			switch(checkPatternValidity(&data, messageType))
			{
			case rdacResultMessageComplete:
				emit statusMessage("Everything OK\nLast RPM update: " + lastMessageReception.value(3).toString("hh:mm:ss.zzz"), Qt::white);
				switch(messageType)
				{
					case 0x01:
						handleMessage1(&data);
						break;
					case 0x02:
						handleMessage2(&data);
						break;
					case 0x03:
						handleMessage3(&data);
						break;
					case 0x04:
						handleMessage4(&data);
						break;
					default:
						data.remove(0, 1);
						break;
				}
				break;
			case rdacResultMessageIncomplete:
				break;
			default:
				emit statusMessage("Found pattern not valid", Qt::yellow);
				break;
			}
		}
		if(!startPatternFound)
		{
			emit statusMessage("No start pattern found yet", Qt::red);
		}
	}
	exec();
}

quint8 RDACconnect::calculateChecksum1(QByteArray data)
{
	quint8 checksum = 0xAA;
	for(int i = 0; i < data.size(); ++i)
	{
		checksum += quint8(data.at(i));
	}
	return checksum;
}

quint8 RDACconnect::calculateChecksum2(QByteArray data)
{
	quint8 checksum = 0x55;
	for(int i = 0; i < data.size(); ++i)
	{
		checksum ^= quint8(data.at(i));
	}
	return checksum;
}

bool RDACconnect::searchStart(QByteArray *data)
{
	while(data->size() >= 3)
	{
		if(quint8(data->at(0)) == 0xD5)
		{
			if(quint8(data->at(1)) == 0x02)
			{
				if((quint8(data->at(2)) >= 0x01) && (quint8(data->at(2)) <= 0x04))
				{
					return true;
				}
			}
		}
		data->remove(0, 1);
	}
	return false;
}

RDACconnect::rdacResults RDACconnect::checkPatternValidity(QByteArray *data, quint8 &messageType)
{
	// Determine and check neccessary size of data
	quint8 requiredSize = 0;
	messageType = quint8(data->at(2));
	switch(messageType)
	{
		case 0x01:
			requiredSize = 9;
			break;
		case 0x02:
			requiredSize = 23;
			break;
		case 0x03:
			requiredSize = 7;
			break;
		case 0x04:
			requiredSize = 29;
			break;
		default:
			data->remove(0, 1);
			return rdacResultMessageIllegalDatatype;
	}
	if(data->size() < requiredSize)
	{
		return rdacResultMessageIncomplete;
	}

	// Calculate and check checksums
	if(quint8(data->at(requiredSize - 2)) == calculateChecksum1(data->mid(2, requiredSize - 4)))
	{
		if(quint8(data->at(requiredSize - 1)) == calculateChecksum2(data->mid(2, requiredSize - 4)))
		{
			return rdacResultMessageComplete;
		}
		else
		{
			qWarning() << "Checksum 2 incorrect" << quint8(data->at(requiredSize - 1));
			data->remove(0, 1);
			return rdacResultMessageInvalidChecksum2;
		}
	}
	else
	{
		qWarning() << "Checksum 1 incorrect" << quint8(data->at(requiredSize - 2));
		data->remove(0, 1);
		return rdacResultMessageInvalidChecksum1;
	}
}

void RDACconnect::handleMessage1(QByteArray *data)
{
	lastMessageReception.insert(1, QDateTime::currentDateTimeUtc());
	RDACmessage1 message;
	memcpy(&message, data->mid(3, 4).constData(), 4);
	data->remove(0, 9);

	qDebug() << Q_FUNC_INFO << "Pulses" << message.pulses;
	qDebug() << Q_FUNC_INFO << "Timing" << message.timing;

	qreal seconds = qreal(lastMessage1.msecsTo(QDateTime::currentDateTimeUtc())) / 1000.0;
	lastMessage1 = QDateTime::currentDateTimeUtc();

	qreal k_factor = 1.0;
	qreal absoluteFuel = qreal(message.pulses) * k_factor / 60.0 / 60.0;
	qreal fuelflow = absoluteFuel / seconds * 60.0 * 60.0;
	emit updateDataMessage1(fuelflow, absoluteFuel);
}

void RDACconnect::handleMessage2(QByteArray *data)
{
	lastMessageReception.insert(2, QDateTime::currentDateTimeUtc());
	RDACmessage2 message;
	memcpy(&message, data->mid(3, 18).constData(), 18);
	data->remove(0, 23);

	double voltage = message.voltage / 4095.0 * 5.0;
	double oilPressure = 0.3320318366 * message.oilPressure - 31.2628022226;
	if(oilPressure < 0.0)
	{
		oilPressure = 0.0;
	}

	qreal insideAirTemperature = qreal(message.internalTemperature) / 100.0;
	qreal outsideAirTemperature = qreal(message.cht1) / 100.0;

	emit updateDataMessage2(insideAirTemperature, outsideAirTemperature, message.cht2, message.oilTemperature, oilPressure, voltage, message.manifoldPressure);
}

void RDACconnect::handleMessage3(QByteArray *data)
{
	lastMessageReception.insert(3, QDateTime::currentDateTimeUtc());
	RDACmessage3 message;
	memcpy(&message, data->mid(3, 2).constData(), 2);
	data->remove(0, 7);

	double revFudge = (6000.0 / 19.6) * 15586.0;
	double rpm = revFudge / message.timeBetweenPulses;
	if(message.timeBetweenPulses > 30000)
	{
		rpm = 0.0;
	}

	emit updateDataMessage3(rpm);
	qDebug() << Q_FUNC_INFO << rpm;
}

void RDACconnect::handleMessage4(QByteArray *data)
{
	lastMessageReception.insert(4, QDateTime::currentDateTimeUtc());
	RDACmessage4 message;
	memcpy(&message, data->mid(3, 24).constData(), 24);

	data->remove(0, 29);

	emit updateDataMessage4egt(message.thermocouple[0], message.thermocouple[1], message.thermocouple[2], message.thermocouple[3]);
	emit updateDataMessage4cht(message.thermocouple[4], message.thermocouple[5], message.thermocouple[6], message.thermocouple[7]);
}
