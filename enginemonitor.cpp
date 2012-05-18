#include "enginemonitor.h"

EngineMonitor::EngineMonitor(QWidget *parent) : QGraphicsView(parent)
  , graphicsScene(this)
{
	setWindowFlags(Qt::FramelessWindowHint);

	graphicsScene.setBackgroundBrush(Qt::black);
	setScene(&graphicsScene);

	setupBarGraphs();

	oilTemperature.setValue(149.0);
	oilPressure.setValue(49.0);
	voltMeter.setValue(14.0);
	ampereMeter.setValue(0.0);
	fuelFlow.setValue(14.9);
	outsideAirTemperature.setValue(24.0);

	QTimer *demoTimer = new QTimer(this);
	connect(demoTimer, SIGNAL(timeout()), this, SLOT(demoFunction()));
	demoTimer->setSingleShot(false);
	demoTimer->start(300);
}

EngineMonitor::~EngineMonitor()
{
}

void EngineMonitor::setupBarGraphs()
{
	oilTemperature.setPos(0, 0);
	oilTemperature.setTitle("OIL T");
	oilTemperature.setUnit("°C");
	oilTemperature.setBorders(80.0, 180.0);
	oilTemperature.addColorStop(ColorStop(Qt::green, 80.0, 160.0));
	oilTemperature.addColorStop(ColorStop(Qt::yellow, 160.0, 180.0));
	graphicsScene.addItem(&oilTemperature);

	oilPressure.setPos(100, 0);
	oilPressure.setTitle("OIL P");
	oilPressure.setUnit("psi");
	oilPressure.setBorders(0.0, 60.0);
	oilPressure.addColorStop(ColorStop(Qt::red, 0.0, 15.0));
	oilPressure.addColorStop(ColorStop(Qt::yellow, 55.0, 60.0));
	oilPressure.addBetweenValue(15.0);
	oilPressure.addBetweenValue(35.0);
	oilPressure.addBetweenValue(50.0);
	graphicsScene.addItem(&oilPressure);

	voltMeter.setPos(0, 150);
	voltMeter.setTitle("VOLTS");
	voltMeter.setUnit("V");
	voltMeter.setBorders(11.5, 18.0);
	voltMeter.addColorStop(ColorStop(Qt::yellow, 11.5, 13.0));
	voltMeter.setPrecision(1, 1);
	graphicsScene.addItem(&voltMeter);

	ampereMeter.setPos(100, 150);
	ampereMeter.setTitle("AMPS");
	ampereMeter.setUnit("A");
	ampereMeter.setBorders(-50.0, 50.0);
	ampereMeter.addColorStop(ColorStop(Qt::red, -50.0, 0.0));
	ampereMeter.addColorStop(ColorStop(Qt::yellow, 30.0, 50.0));
	ampereMeter.addBetweenValue(0.0);
	graphicsScene.addItem(&ampereMeter);

	fuelFlow.setPos(0, 300);
	fuelFlow.setTitle("FF");
	fuelFlow.setUnit("lph");
	fuelFlow.setBorders(0, 30);
	fuelFlow.addBetweenValue(10.0);
	fuelFlow.setPrecision(1);
	fuelFlow.addBetweenValue(20.0);
	graphicsScene.addItem(&fuelFlow);

	outsideAirTemperature.setPos(100, 300);
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
	static double oilTemp = 160.0;
	if(oilTemp < 120.0)
	{
		oilTemp = 160.0;
	}
	oilTemp -= 1.0;
	oilTemperature.setValue(oilTemp);

	static double oilPress = 15.0;
	if(oilPress > 35.0)
	{
		oilPress = 15.0;
	}
	oilPress += 0.5;
	oilPressure.setValue(oilPress);

	static double volts = 12.0;
	if(volts > 16.0)
	{
		volts = 12.0;
	}
	volts += 0.1;
	voltMeter.setValue(volts);

	static double amperes = -5.0;
	if(amperes > 27.0)
	{
		amperes = -5.0;
	}
	amperes += 1.0;
	ampereMeter.setValue(amperes);

	static double flow = 25.0;
	if(flow < 5.0)
	{
		flow = 25.0;
	}
	flow -= 0.5;
	fuelFlow.setValue(flow);

	static double airTemp = 0.0;
	if(airTemp > 27.0)
	{
		airTemp = 0.0;
	}
	airTemp += 0.1;
	outsideAirTemperature.setValue(airTemp);
}
