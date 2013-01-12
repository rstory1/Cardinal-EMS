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

#include <QtSvg>

#include "enginemonitor.h"

EngineMonitor::EngineMonitor(QWidget *parent) : QGraphicsView(parent)
  , graphicsScene(this)
{
	//Initializing the window behaviour and it's scene
	setWindowFlags(Qt::FramelessWindowHint);
	graphicsScene.setBackgroundBrush(Qt::black);
	setScene(&graphicsScene);
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	//Setting up the items to be displayed
	setupRpmIndicator();
	setupExhaustGasTemperature();
	setupCylinderHeadTemperature();
	setupBarGraphs();
	graphicsScene.update();

//	//Demo timer, for testing purposes only
//	QTimer *demoTimer = new QTimer(this);
//	connect(demoTimer, SIGNAL(timeout()), this, SLOT(demoFunction()));
//	demoTimer->setSingleShot(false);
//	demoTimer->start(200);
}

EngineMonitor::~EngineMonitor()
{
}

void EngineMonitor::setupRpmIndicator()
{
	rpmIndicator.setPos(-350, -100);
	rpmIndicator.setStartSpan(230.0, 240.0);
	rpmIndicator.setBorders(0.0, 2800.0, 300.0, 2550.0);
	rpmIndicator.addBetweenValue(0.0);
	rpmIndicator.addBetweenValue(300.0);
	rpmIndicator.addBetweenValue(600.0);
	rpmIndicator.addBetweenValue(900.0);
	rpmIndicator.addBetweenValue(1200.0);
	rpmIndicator.addBetweenValue(1500.0);
	rpmIndicator.addBetweenValue(1800.0);
	rpmIndicator.addBetweenValue(2100.0);
	rpmIndicator.addBetweenValue(2400.0);
	rpmIndicator.addBetweenValue(2700.0);
	graphicsScene.addItem(&rpmIndicator);
}

void EngineMonitor::setupExhaustGasTemperature()
{
	exhaustGasTemperature.setPos(-475, 175);
	exhaustGasTemperature.setBorders(200.0, 1700.0, 1600.0, 1650.0);
	exhaustGasTemperature.addBetweenValue(400);
	exhaustGasTemperature.addBetweenValue(800);
	exhaustGasTemperature.addBetweenValue(1200);
	exhaustGasTemperature.addBetweenValue(1600);
//	exhaustGasTemperature.addBetweenValue(1650);
	exhaustGasTemperature.addBetweenValue(1700);
	exhaustGasTemperature.setLeanWindow(200.0);
	graphicsScene.addItem(&exhaustGasTemperature);
}

void EngineMonitor::setupCylinderHeadTemperature()
{
	cylinderHeadTemperature.setPos(-200, 175);
	cylinderHeadTemperature.setBorders(100.0, 250.0, 200.0, 230.0);
	cylinderHeadTemperature.addBetweenValue(120);
	cylinderHeadTemperature.addBetweenValue(160);
	cylinderHeadTemperature.addBetweenValue(200);
	cylinderHeadTemperature.addBetweenValue(230);
	graphicsScene.addItem(&cylinderHeadTemperature);
}

void EngineMonitor::setupBarGraphs()
{
	oilTemperature.setPos(0, -150);
	oilTemperature.setTitle("OIL T");
	oilTemperature.setUnit("°C");
	oilTemperature.setBorders(80.0, 180.0);
	oilTemperature.addColorStop(ColorStop(Qt::green, 80.0, 160.0));
	oilTemperature.addColorStop(ColorStop(Qt::yellow, 160.0, 180.0));
	graphicsScene.addItem(&oilTemperature);

	oilPressure.setPos(100, -150);
	oilPressure.setTitle("OIL P");
	oilPressure.setUnit("psi");
	oilPressure.setBorders(0.0, 60.0);
	oilPressure.addColorStop(ColorStop(Qt::red, 0.0, 15.0));
	oilPressure.addColorStop(ColorStop(Qt::yellow, 55.0, 60.0));
	oilPressure.addBetweenValue(15.0);
	oilPressure.addBetweenValue(35.0);
	oilPressure.addBetweenValue(50.0);
	graphicsScene.addItem(&oilPressure);

	voltMeter.setPos(0, 0);
	voltMeter.setTitle("VOLTS");
	voltMeter.setUnit("V");
	voltMeter.setBorders(11.5, 18.0);
	voltMeter.addColorStop(ColorStop(Qt::yellow, 11.5, 13.0));
	voltMeter.setPrecision(1, 1);
	graphicsScene.addItem(&voltMeter);

	ampereMeter.setPos(100, 0);
	ampereMeter.setTitle("AMPS");
	ampereMeter.setUnit("A");
	ampereMeter.setBorders(-50.0, 50.0);
	ampereMeter.addColorStop(ColorStop(Qt::red, -50.0, 0.0));
	ampereMeter.addColorStop(ColorStop(Qt::yellow, 30.0, 50.0));
	ampereMeter.addBetweenValue(0.0);
	graphicsScene.addItem(&ampereMeter);

	fuelFlow.setPos(0, 150);
	fuelFlow.setTitle("FF");
	fuelFlow.setUnit("lph");
	fuelFlow.setBorders(0, 30);
	fuelFlow.addBetweenValue(10.0);
	fuelFlow.setPrecision(1);
	fuelFlow.addBetweenValue(20.0);
	graphicsScene.addItem(&fuelFlow);

	outsideAirTemperature.setPos(100, 150);
	outsideAirTemperature.setTitle("OAT");
	outsideAirTemperature.setUnit("°C");
	outsideAirTemperature.setBorders(-10.0, 40);
	outsideAirTemperature.addBetweenValue(0.0);
	outsideAirTemperature.addBetweenValue(10.0);
	outsideAirTemperature.addBetweenValue(20.0);
	outsideAirTemperature.addBetweenValue(30.0);
	graphicsScene.addItem(&outsideAirTemperature);
}

void EngineMonitor::demoFunction()
{
	qsrand(QDateTime::currentDateTime().toTime_t());
	static double rpm = 0.0;
	rpm += 5.0;
	if(rpm > 2800.0)
	{
		saveSceneToSvg("./out/maxRPM.svg");
		rpm = 0.0;
	}
	rpmIndicator.setValue(rpm);

	static double basicEGT = 0.0;
	static bool egtUp = true;
	static bool leaned = false;
	static double off13 = 0.0;
	static double off24 = 0.0;
	if(leaned)
	{
		if(basicEGT < 1190.0)
		{
			off13 = double(qrand())/double(RAND_MAX)*3.0;
			off24 = double(qrand())/double(RAND_MAX)*5.0;
		}
	}
	else if(egtUp)
	{
		basicEGT += 1.0;
	}
	else
	{
		basicEGT -= 1.0;
	}
	if(basicEGT > 1200.0 && egtUp)
	{
		egtUp = false;
	}
	if(basicEGT < 1150.0 && !egtUp)
	{
		leaned = true;
		egtUp = true;
	}
	exhaustGasTemperature.setValues(basicEGT+20.0+off13, basicEGT+10.0-off24, basicEGT+5.0-off13, basicEGT+30.0+off24);

	static double basicCHT = 0.0;
	basicCHT += 1.0;
	if(basicCHT > 250.0)
	{
		basicCHT = 0.0;
	}
	static double offset1 = double(qrand())/double(RAND_MAX)*25.0;
	static double offset2 = double(qrand())/double(RAND_MAX)*7.0;
	static double offset3 = double(qrand())/double(RAND_MAX)*15.0;
	static double offset4 = double(qrand())/double(RAND_MAX)*9.0;
	cylinderHeadTemperature.setValues(basicCHT+offset1,
									  basicCHT+offset2,
									  basicCHT+offset3,
									  basicCHT+offset4);

	static double oilTemp = 160.0;
	if(oilTemp < 120.0)
	{
		oilTemp = 160.0;
	}
	oilTemp -= 0.1;
	oilTemperature.setValue(oilTemp);

	static double oilPress = 0.0;
	oilPress += 0.05;
	if(oilPress > 60.0)
	{
		oilPress = 0.0;
	}
	oilPressure.setValue(oilPress);

	static double volts = 11.5;
	volts += 0.01;
	if(volts > 18.0)
	{
		volts = 11.5;
	}
	voltMeter.setValue(volts);

	static double amperes = 50.0;
	amperes -= 0.1;
	if(amperes < -50.0)
	{
		amperes = 50.0;
	}
	ampereMeter.setValue(amperes);

	static double flow = 30.0;
	flow -= 0.05;
	if(flow < 0.0)
	{
		flow = 30.0;
	}
	fuelFlow.setValue(flow);

	static double airTemp = -10.0;
	airTemp += 0.01;
	if(airTemp > 40.0)
	{
		airTemp = -10.0;
	}
	outsideAirTemperature.setValue(airTemp);
}

void EngineMonitor::saveSceneToSvg(const QString fileName)
{
	QSvgGenerator generator;
	generator.setFileName(fileName);
	generator.setSize(QSize(800, 600));
	generator.setViewBox(QRect(0, 0, 800, 600));
	generator.setTitle(tr("SVG Generator Example Drawing"));
	generator.setDescription(tr("An SVG drawing created by the SVG Generator"));
	QPainter painter;
	painter.begin(&generator);
	graphicsScene.render(&painter);
	painter.end();
}
