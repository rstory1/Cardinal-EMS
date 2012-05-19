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

#ifndef ENGINEMONITOR_H
#define ENGINEMONITOR_H

#include <QtGui>

#include "rpmindicator.h"
#include "exhaustgastemperature.h"
#include "cylinderheadtemperature.h"
#include "bargraph.h"

class EngineMonitor : public QGraphicsView
{
	Q_OBJECT
public:
	EngineMonitor(QWidget *parent = 0);
	~EngineMonitor();
private:
	void setupRpmIndicator();
	void setupBarGraphs();
	void setupExhaustGasTemperature();
	void setupCylinderHeadTemperature();
	void saveSceneToSvg(const QString fileName = "./out/output.svg");
	QGraphicsScene graphicsScene;
	RpmIndicator rpmIndicator;
	ExhaustGasTemperature exhaustGasTemperature;
	CylinderHeadTemperature cylinderHeadTemperature;
	BarGraph oilTemperature;
	BarGraph oilPressure;
	BarGraph voltMeter;
	BarGraph ampereMeter;
	BarGraph fuelFlow;
	BarGraph outsideAirTemperature;
private slots:
	void demoFunction();
};

#endif // ENGINEMONITOR_H
