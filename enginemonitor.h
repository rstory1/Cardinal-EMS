#ifndef ENGINEMONITOR_H
#define ENGINEMONITOR_H

#include <QtGui>

#include "rpmindicator.h"
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
	QGraphicsScene graphicsScene;
	RpmIndicator rpmIndicator;
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
