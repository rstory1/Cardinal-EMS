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

	//Demo timer, for testing purposes only
	QTimer *demoTimer = new QTimer(this);
	connect(demoTimer, SIGNAL(timeout()), this, SLOT(demoFunction()));
	demoTimer->setSingleShot(false);
	demoTimer->start(20);
}

EngineMonitor::~EngineMonitor()
{
}

void EngineMonitor::setupRpmIndicator()
{
	rpmIndicator.setPos(-350, 0);
	rpmIndicator.setBorders(0.0, 2800.0, 300.0, 2550.0);
	rpmIndicator.addBetweenValue(0.0);
	rpmIndicator.addBetweenValue(600.0);
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
	exhaustGasTemperature.setPos(-475, 295);
	exhaustGasTemperature.setBorders(200.0, 1700.0, 1600.0, 1650.0);
	exhaustGasTemperature.addBetweenValue(400);
	exhaustGasTemperature.addBetweenValue(800);
	exhaustGasTemperature.addBetweenValue(1200);
	exhaustGasTemperature.addBetweenValue(1600);
	exhaustGasTemperature.addBetweenValue(1650);
	exhaustGasTemperature.setLeanWindow(200.0);
	graphicsScene.addItem(&exhaustGasTemperature);
}

void EngineMonitor::setupCylinderHeadTemperature()
{
	cylinderHeadTemperature.setPos(-200, 295);
	cylinderHeadTemperature.setBorders(100.0, 250.0, 200.0, 230.0);
	cylinderHeadTemperature.addBetweenValue(120);
	cylinderHeadTemperature.addBetweenValue(160);
	cylinderHeadTemperature.addBetweenValue(200);
	cylinderHeadTemperature.addBetweenValue(230);
	graphicsScene.addItem(&cylinderHeadTemperature);
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
	static double rpm = 2400.0;
	if(rpm > 2750.0)
	{
		rpm = 2400.0;
	}
	rpm += 1.0;
	rpmIndicator.setValue(rpm);

	static double basicEGT = 800.0;
	static bool egtUp = true;
	if(basicEGT > 1200.0 && egtUp)
	{
		egtUp = false;
	}
	if(basicEGT < 1000.0 && !egtUp)
	{
		egtUp = true;
		basicEGT = 800.0;
	}
	if(egtUp)
	{
		basicEGT += 1.0;
	}
	else
	{
		basicEGT -= 1.0;
	}
	exhaustGasTemperature.setValues(basicEGT+20.0, basicEGT+10.0, basicEGT+5.0, basicEGT+30.0);

	static double basicCHT = 50.0;
	if(basicCHT > 220.0)
	{
		basicCHT = 50.0;
	}
	basicCHT += 1.0;
	cylinderHeadTemperature.setValues(basicCHT+20.0, basicCHT+10.0, basicCHT+5.0, basicCHT+30.0);

	static double oilTemp = 160.0;
	if(oilTemp < 120.0)
	{
		oilTemp = 160.0;
	}
	oilTemp -= 0.1;
	oilTemperature.setValue(oilTemp);

	static double oilPress = 15.0;
	if(oilPress > 35.0)
	{
		oilPress = 15.0;
	}
	oilPress += 0.05;
	oilPressure.setValue(oilPress);

	static double volts = 12.0;
	if(volts > 16.0)
	{
		volts = 12.0;
	}
	volts += 0.01;
	voltMeter.setValue(volts);

	static double amperes = -5.0;
	if(amperes > 27.0)
	{
		amperes = -5.0;
	}
	amperes += 0.1;
	ampereMeter.setValue(amperes);

	static double flow = 30.0;
	flow -= 0.05;
	if(flow < 0.0)
	{
		flow = 30.0;
	}
	fuelFlow.setValue(flow);

	static double airTemp = 0.0;
	if(airTemp > 27.0)
	{
		airTemp = 0.0;
	}
	airTemp += 0.01;
	outsideAirTemperature.setValue(airTemp);
}
