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
