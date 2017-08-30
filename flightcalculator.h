//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Cardinal EMS, Experimental Aircraft EMS                              //
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
#ifndef FLIGHTCALCULATOR_H
#define FLIGHTCALCULATOR_H

#include <QtCore>
#include <QObject>
#include <math.h>

#define PI 3.14159265

//! flightCalculator Class
/*!
 * This class runs various calculations from engine/GPS data to be displayed
*/

class flightCalculator : public QThread
{
    Q_OBJECT
public:
    explicit flightCalculator(QObject *parent = 0);
    void run();
    void setTas(float tas);
    void setMagHdg(float magHdg);
    void setGs(float gs);
    void setTrack(float track);

private:
    float tas;
    float magHdg;
    float gs;
    float track;
    float windSpd;
    float windDir;
    float fuelFlow;
    float wca;

    void calculateWindVector();

signals:
    void updateWindVector(float dir, float spd, float mHdg);

public slots:
    void onSpeedAndHeadingUpdate(/*float trueas, float magneticHdg, float grounds, float gpsTrack*/);
};

#endif // FLIGHTCALCULATOR_H
