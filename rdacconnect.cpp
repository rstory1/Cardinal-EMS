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

void RDACconnect::run()
{
	forever
	{
		static double oilT = 180.0;
		static double oilP = 0.0;
		static double flow = 0.0;
		static double volt = 24.0;
		oilT -= 1.0;
		oilP += 0.5;
		flow += 0.1;
		volt -= 0.3;
		emit updateDataMessage1(flow);
		emit updateDataMessage2(oilT, oilP, volt);
		QByteArray datatest;
		datatest.resize(12);
		datatest[0] = quint8(0x3c);
		datatest[1] = quint8(0xb8);
		datatest[2] = quint8(0xD5);
		datatest[3] = quint8(0x02);
		datatest[4] = quint8(0x01);
		datatest[5] = quint8(0xff);
		datatest[6] = quint8(0xff);
		datatest[7] = quint8(0xff);
		datatest[8] = quint8(0xff);
		datatest[9] = quint8(calculateChecksum1(datatest.mid(4, 5)));
		datatest[10] = quint8(calculateChecksum2(datatest.mid(4, 5)));
		datatest[11] = quint8(0x05);

		if(searchStart(&datatest))
		{
			quint8 messageType = 0x00;
			if(checkPatternValidity(&datatest, messageType))
			{
				switch(messageType)
				{
				case 0x01:
					handleMessage1(&datatest);
					break;
				case 0x02:
					handleMessage2(&datatest);
					break;
				default:
					break;
				}
			}
		}
		sleep(1);
	}
	exec();
}

quint8 RDACconnect::calculateChecksum1(QByteArray data)
{
	quint8 checksum = 0x55;
	for(int i = 0; i < data.size(); ++i)
	{
		checksum += quint8(data.at(i));
	}
	return checksum;
}

quint8 RDACconnect::calculateChecksum2(QByteArray data)
{
	quint8 checksum = 0xAA;
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

bool RDACconnect::checkPatternValidity(QByteArray *data, quint8 &messageType)
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
		return false;
	}
	if(data->size() < requiredSize)
	{
		return false;
	}

	// Calculate and check checksums
	if(quint8(data->at(requiredSize - 2)) == calculateChecksum1(data->mid(2, requiredSize - 4)))
	{
		if(quint8(data->at(requiredSize - 1)) == calculateChecksum2(data->mid(2, requiredSize - 4)))
		{
			return true;
		}
		else
		{
			qWarning() << "Checksum 2 incorrect" << quint8(data->at(requiredSize - 1));
			return false;
		}
	}
	else
	{
		qWarning() << "Checksum 1 incorrect" << quint8(data->at(requiredSize - 2));
		return false;
	}
	return false;
}

void RDACconnect::handleMessage1(QByteArray *data)
{
	RDACmessage1 message;
	memcpy(&message, data->mid(3, 4).constData(), 4);

	quint16 seconds = lastMessage1.secsTo(QDateTime::currentDateTimeUtc());

	qreal k_factor;
	qreal fuelflow = qreal(message.pulses) * k_factor / qreal(seconds);
	data->remove(0, 9);
}

void RDACconnect::handleMessage2(QByteArray *data)
{
	RDACmessage2 message;
	memcpy(&message, data->mid(3, 4).constData(), 18);
	data->remove(0, 23);
}
