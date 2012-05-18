#ifndef ENGINEMONITOR_H
#define ENGINEMONITOR_H

#include <QtGui>

#include "bargraph.h"

class EngineMonitor : public QGraphicsView
{
	Q_OBJECT
public:
	EngineMonitor(QWidget *parent = 0);
	~EngineMonitor();
private:
	void setupBarGraphs();
	QGraphicsScene graphicsScene;
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
