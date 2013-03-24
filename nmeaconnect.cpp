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

#include "nmeaconnect.h"

NMEAconnect::NMEAconnect(QObject *parent) : QThread(parent)
  , settings("./settings.ini", QSettings::IniFormat, parent)
{
}

void NMEAconnect::run()
{
	QString portString = "\\\\.\\";
	portString.append(settings.value("SkyMap/Port", "COM4").toString());
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
		emit userMessage("Sky Map COM error", "Unable to open " + portString + '\n' + "Settings file: " + settings.fileName() + '\n' + "Closing Application", true);
		exec();
	}

	QString data;
	forever
	{
		char character;
		DWORD nrBytes = 0;
		if(ReadFile(serialhCom, &character, 1, &nrBytes, NULL))
		{
			if(nrBytes == 1)
			{
				data.append(character);
			}
		}
		else
		{
			emit userMessage("Sky Map COM error", "Error reading data, closing application", true);
			exec();
		}
		searchMessage(&data);
	}
	exec();
}

void NMEAconnect::searchMessage(QString *data)
{
	while(data->contains("\r\n"))
	{
		if(data->startsWith("$GPRMB"))
		{
			QString message = data->split("\r\n").first();
			data->remove(0, message.size());
			handleMessageRMB(message);
		}
		data->remove(0, 1);
	}
}

void NMEAconnect::handleMessageRMB(QString data)
{
	data.remove(0, 1);
	QString message = data.split('*').first();
	quint8 checksum = data.split('*').last().toInt(0, 16);
	if(calculateChecksum(message) == checksum)
	{
		QStringList dataList = message.split(',');
		double distance = dataList.value(10).toDouble();
		double speed = dataList.value(12).toDouble();
		double time = distance/speed;
		emit newTimeToDestination(time);
	}
}

char NMEAconnect::calculateChecksum(QString data)
{
	char checksum = data.at(0).toAscii();
	while(data.size() > 1)
	{
		data.remove(0, 1);
		checksum ^= data.at(0).toAscii();
	}
	return checksum;
}
