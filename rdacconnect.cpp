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

void RDACconnect::run()
{
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

bool RDACconnect::checkPatternValidity(QByteArray *data)
{
	// Determine and check neccessary size of data
	quint8 requiredSize = 0;
	switch(quint8(data->at(2)))
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
