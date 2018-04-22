//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2017 Ryan Story                                        //
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

#include "flightcalculator.h"

flightCalculator::flightCalculator(QObject *parent) : QThread(parent)
{

}

void flightCalculator::run() {
    exec();
}

void flightCalculator::calculateWindVector() {
    float gs_x = gs * cos((450-track)*PI/180);
    float gs_y = gs * sin((450-track)*PI/180);
    float tas_x = -tas * cos((450-magHdg)*PI/180);
    float tas_y = -tas * sin((450-magHdg)*PI/180);

    float ws_x = gs_x + tas_x;
    float ws_y = gs_y + tas_y;

    windSpd = sqrt(ws_x*ws_x + ws_y*ws_y);

    if (ws_x > 0) {
        if (ws_y >= 0) {
            windDir = (atan(ws_y/ws_x))*180.0/PI;
        } else if (ws_y < 0) {
            windDir = (atan(ws_y/ws_x))*180.0/PI - (-1.0)*360;
        }
    } else if (ws_x < 0) {
        if (ws_y >= 0) {
            windDir = 180.0 - (atan(-ws_y/ws_x))*180.0/PI;
        } else if (ws_y < 0) {
            windDir = -180.0 - (-1.0)*(atan(ws_y/ws_x))*180.0/PI - (-1.0)*360.0;
        }
    }

    windDir = 450.0 - windDir;

    if (windDir >= 360.0) {
        windDir = windDir - 360.0;
    }
   // qDebug() << windDir << "!";
    if (windDir <=180) {
        windDir = windDir +180;
    } else {
        windDir = windDir - 180;
    }

    //qDebug() << windDir << ";" << (windDir - magHdg);

    updateWindVector(windSpd, windDir, magHdg);
}

void flightCalculator::onSpeedAndHeadingUpdate(/*float trueas, magneticHdg, grounds, gpsTrack*/) {
    tas = 145.0;//trueas;
    magHdg = 30.0; //magneticHdg;
    gs = 125.0;//grounds;
    track = 15.0;//gpsTrack;

    calculateWindVector();
}
