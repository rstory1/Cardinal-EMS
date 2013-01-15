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
