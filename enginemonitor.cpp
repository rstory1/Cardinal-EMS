//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2012 Tobias Rad                                        //
//                2017 Ryan Story                                       //
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

//#include <QtSvg>

#include "enginemonitor.h"

EngineMonitor::EngineMonitor(QWidget *parent) : QGraphicsView(parent)
  , settings(QCoreApplication::applicationDirPath() + "/ems/settings/settings.ini", QSettings::IniFormat, parent)
  , gaugeSettings(QCoreApplication::applicationDirPath() + "/ems/settings/gaugeSettings.ini", QSettings::IniFormat, parent)
{
	//Initializing the window behaviour and it's scene
    setWindowFlags(Qt::FramelessWindowHint);
    setScene(&ems_full);
    currentScene = "emsFull";
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

	//Setting up the items to be displayed
    setupuserSettings();

    this->mapToScene(this->rect());
    this->setFrameShape(QGraphicsView::NoFrame);

    this->scene()->setSceneRect(0,0,800,480);
    buttonBar.setPos(0,this->scene()->height());
    this->scene()->addItem(&buttonBar);

    //  Get the interface type, Arduino or RDAC
    sensorInterfaceType = settings.value("Sensors/interface", "arduino").toString();

    // Get the temp for when the engine is warmed up
    warmupTemp=gaugeSettings.value("OilTemp/warmupTemp").toInt();

//    // Plot stuff
//    customPlot = new QCustomPlot();
//    customPlot->setStyleSheet("border: 8px solid red;background-color: yellow");

//    QGraphicsProxyWidget *test;
//    test = new QGraphicsProxyWidget();
//    test->setWidget(customPlot);
//    test->setPos(0, 200);

//    //this->scene()->addItem(test);

//    customPlot->setFixedHeight(150);
//    customPlot->setFixedWidth(300);
//    customPlot->addGraph(); // blue line
//    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
//    customPlot->addGraph(); // red line
//    customPlot->graph(1)->setPen(QPen(Qt::green));
//    customPlot->addGraph(); // red line
//    customPlot->graph(2)->setPen(QPen(QColor(255, 110, 40)));
//    customPlot->addGraph(); // red line
//    customPlot->graph(3)->setPen(QPen(Qt::yellow));

//    QVector<double> ticks;
//    QVector<QString> labels;
//    ticks << 1 << 2 << 3 << 4 << 5;
//    labels << "2:00" << "1:30" << "1:00" << "00:30" << "00:00";
//    //QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    //timeTicker->setTimeFormat("%m:%s");
//    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
//    textTicker->addTicks(ticks, labels);
//    customPlot->xAxis->setTicker(textTicker);
//    customPlot->axisRect()->setupFullAxesBox();
//    customPlot->yAxis->setRange(0, 300);
//    customPlot->setBackground(Qt::black);
//    customPlot->yAxis->setTickLabelColor(Qt::white);
//    customPlot->xAxis->setTickLabelColor(Qt::white);
//    customPlot->xAxis->setTicks(false);
//    customPlot->xAxis->grid()->setVisible(false);


    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
//    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
//    dataTimer.start(1000); // Interval 0 means to refresh as fast as possible

    // End plot stuff

    setupLogFile();

	//Demo timer, for testing purposes only
#ifdef QT_DEBUG
    QTimer *demoTimer = new QTimer(this);
    connect(demoTimer, SIGNAL(timeout()), this, SLOT(demoFunction()));
    demoTimer->setSingleShot(false);
    demoTimer->start(200);
#endif

    //socket = new QUdpSocket(this);

    //qDebug()<< socket->bind(QHostAddress("192.168.1.120"), 49901);

    //connect(socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));

    //qDebug()<<"Creating";
    //qDebug()<<socket->BoundState;

    // Initialize the timer to flash values on alarm
    // QTimer *flashTimer = new QTimer(this);
    flashTimer.start(1000);

    // Initialize the timer for the Hobbs and Flight time
    //clockTimer = new QTimer(this);
    clockTimer.start(1000);

    qDebug()<<"Enter connectSignals()";
    connectSignals();
    qDebug() << "Returned from connectSignals()";
}

EngineMonitor::~EngineMonitor()
{
	logFile->close();
}

void EngineMonitor::setupLogFile()
{
    QDir dir(QApplication::applicationDirPath() + "/ems/engineLogs");
    if (!dir.exists())
        dir.mkpath(".");
    logFile = new QFile(QString(QApplication::applicationDirPath() + "/ems/engineLogs/EngineData ").append(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh.mm.ss")).append(".csv"), this);
	if(logFile->open(QIODevice::WriteOnly))
	{
		QTimer *writeLogFileTimer = new QTimer(this);
		connect(writeLogFileTimer, SIGNAL(timeout()), this, SLOT(writeLogFile()));
		writeLogFileTimer->setSingleShot(false);
		writeLogFileTimer->start(settings.value("Logging/SampleRate", 1).toInt() * 1000);
	}
	else
	{
        qDebug() << QString(QApplication::applicationDirPath() + "/ems/engineLogs/EngineData ");
		userMessageHandler("Unable to open log file", "Unable to open log file, closing application.", true);
	}

	logFile->write("[Header]\r\n");
	logFile->write("Created with Cardinal EMS - Build BETA\r\n");
	logFile->write(QString("Call Sign: %1\r\n").arg(settings.value("Aircraft/CALL_SIGN").toString()).toLatin1());
	logFile->write(QString("Aircraft Model: %1\r\n").arg(settings.value("Aircraft/AIRCRAFT_MODEL").toString()).toLatin1());
	logFile->write(QString("Aircraft S/N: %1\r\n").arg(settings.value("Aircraft/AIRCRAFT_SN").toString()).toLatin1());
	logFile->write(QString("Engine Type: %1\r\n").arg(settings.value("Aircraft/ENGINE_TYPE").toString()).toLatin1());
	logFile->write(QString("Engine S/N: %1\r\n").arg(settings.value("Aircraft/ENGINE_SN").toString()).toLatin1());
    logFile->write(QString("All temperatures in degree %1\r\n oil pressure in %2\r\n fuel flow in %3.\r\n").arg(settings.value("Units/temp/", "F").toString(),settings.value("Units/pressure","psi").toString(),settings.value("Units/fuelFlow","gph").toString()).toLatin1());
	logFile->write("[data]\r\n");
    logFile->write("INDEX;TIME;EGT1;EGT2;EGT3;EGT4;CHT1;CHT2;CHT3;CHT4;OILT;OILP;OAT;IAT;BAT;CUR;RPM;MAP;FF;HOBBS;FLIGHT;MARK\r\n");
}

void EngineMonitor::writeLogFile()
{
	static quint64 sample = 0;
	logFile->write(QString::number(sample).append(';').toLatin1());
	logFile->write(QDateTime::currentDateTimeUtc().toString("yyyy-dd-MM hh:mm:ss").append(';').toLatin1());
    QList<double> egtValues = chtEgt.getCurrentEgtValues();
	logFile->write(QString::number(egtValues.value(0, 0.0), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(egtValues.value(1, 0.0), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(egtValues.value(2, 0.0), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(egtValues.value(3, 0.0), 'f', 0).append(';').toLatin1());
    QList<double> chtValues = chtEgt.getCurrentChtValues();
	logFile->write(QString::number(chtValues.value(0, 0.0), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(chtValues.value(1, 0.0), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(chtValues.value(2, 0.0), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(chtValues.value(3, 0.0), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(oilTemperature.getValue(), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(oilPressure.getValue(), 'f', 1).append(';').toLatin1());
	logFile->write(QString::number(outsideAirTemperature.getValue(), 'f', 1).append(';').toLatin1());
	logFile->write(QString::number(insideAirTemperature.getValue(), 'f', 1).append(';').toLatin1());
	logFile->write(QString::number(voltMeter.getValue(), 'f', 1).append(';').toLatin1());
	logFile->write(QString::number(ampereMeter.getValue(), 'f', 1).append(';').toLatin1());
	logFile->write(QString::number(rpmIndicator.getValue(), 'f', 0).append(';').toLatin1());
	logFile->write(QString::number(manifoldPressure.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(fuelFlow.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString(hobbs.getHobbsTime().append(';')).toLatin1());
    logFile->write(QString(hobbs.getFlightTime().append(';')).toLatin1());
    logFile->write("\r\n");
	logFile->flush();
	++sample;
}

void EngineMonitor::setupAlarm()
{
    alarmWindow.setPos(50, 100);
    graphicsScene.addItem(&alarmWindow);
    alarmWindow.setVisible(false);
}

void EngineMonitor::setupRpmIndicator()
{
    double minValue, maxValue;
    minValue = gaugeSettings.value("RPM/min",0).toInt();
    maxValue = gaugeSettings.value("RPM/max",0).toInt();
    rpmIndicator.setPos(450, 140);
	rpmIndicator.setStartSpan(230.0, 240.0);
    rpmIndicator.setBorders(minValue, maxValue);

    for(int i=0;(i<=maxValue);i=i+1000) {
        rpmIndicator.addBetweenValue(i);
    }
	graphicsScene.addItem(&rpmIndicator);
}

//void EngineMonitor::setupExhaustGasTemperature()
//{
//	exhaustGasTemperature.setPos(-475, 175);
//	exhaustGasTemperature.setBorders(300.0, 850.0, 750.0, 800.0);
//	exhaustGasTemperature.addBetweenValue(450);
//	exhaustGasTemperature.addBetweenValue(550);
//	exhaustGasTemperature.addBetweenValue(650);
//	exhaustGasTemperature.addBetweenValue(750);
//	exhaustGasTemperature.addBetweenValue(800);
//	exhaustGasTemperature.addBetweenValue(850);
//	exhaustGasTemperature.setLeanWindow(200.0);
//    graphicsScene.addItem(&exhaustGasTemperature);
//}

//void EngineMonitor::setupCylinderHeadTemperature()
//{
//	cylinderHeadTemperature.setPos(-200, 175);
//	cylinderHeadTemperature.setBorders(60.0, 160.0, 140.0, 150.0);
//	cylinderHeadTemperature.addBetweenValue(80);
//	cylinderHeadTemperature.addBetweenValue(100);
//	cylinderHeadTemperature.addBetweenValue(120);
//	cylinderHeadTemperature.addBetweenValue(140);
//	cylinderHeadTemperature.addBetweenValue(150);
//	cylinderHeadTemperature.addBetweenValue(160);
//	graphicsScene.addItem(&cylinderHeadTemperature);

//    chtEgt.addBetweenValue(80);
//    chtEgt.addBetweenValue(100);
//    chtEgt.addBetweenValue(120);
//    chtEgt.addBetweenValue(140);
//    chtEgt.addBetweenValue(150);
//    chtEgt.addBetweenValue(160);
//}

void EngineMonitor::setupChtEgt()
{
    chtEgt.setPos(700, 450);
    graphicsScene.addItem(&chtEgt);
}

void EngineMonitor::setupBarGraphs()
{
    oilTemperature.setPos(620, 60);
    oilTemperature.setTitle("OIL T");
    oilTemperature.setUnit(settings.value("Units/temp").toString().toLatin1());
    oilTemperature.setIndicatorSide("left");
    oilTemperature.setGaugeType("OilTemp");
    graphicsScene.addItem(&oilTemperature);

    oilPressure.setPos(690, 60);
    oilPressure.setTitle("OIL P");
    oilPressure.setUnit(settings.value("Units/pressure").toString().toLatin1());
    oilPressure.setGaugeType("OilPress");
    graphicsScene.addItem(&oilPressure);

    voltMeter.setPos(760, 60);
    voltMeter.setTitle("VOLTS");
    voltMeter.setUnit("V");
    voltMeter.setPrecision(1, 1);
    voltMeter.setIndicatorSide("left");
    voltMeter.setGaugeType("Volt");
    graphicsScene.addItem(&voltMeter);

    ampereMeter.setPos(690, 200);
    ampereMeter.setTitle("AMPS");
    ampereMeter.setUnit("A");
    ampereMeter.addBetweenValue(0.0);
    ampereMeter.setGaugeType("Amp");
    graphicsScene.addItem(&ampereMeter);

    fuelFlow.setPos(760, 200);
    fuelFlow.setTitle("FF");
    fuelFlow.setUnit(settings.value("Units/fuelFlow").toString().toLatin1());
    fuelFlow.setPrecision(1);
    fuelFlow.setIndicatorSide("left");
    fuelFlow.setGaugeType("Fuel");
    graphicsScene.addItem(&fuelFlow);
    fuelFlow.setVisible(false);

    insideAirTemperature.setPos(800, 200);
    insideAirTemperature.setTitle("IAT");
    insideAirTemperature.setUnit(settings.value("Units/temp").toString().toLatin1());
    insideAirTemperature.setPrecision(1);
    graphicsScene.addItem(&insideAirTemperature);
    insideAirTemperature.setVisible(false);
    connect(&outsideAirTemperature, SIGNAL(hasBeenClicked()), &outsideAirTemperature, SLOT(makeInvisible()));
    connect(&outsideAirTemperature, SIGNAL(hasBeenClicked()), &insideAirTemperature, SLOT(makeVisible()));

    outsideAirTemperature.setPos(850, 350);
    outsideAirTemperature.setTitle("OAT");
    outsideAirTemperature.setUnit(settings.value("Units/temp").toString().toLatin1());
    outsideAirTemperature.setPrecision(1);
    graphicsScene.addItem(&outsideAirTemperature);
    connect(&insideAirTemperature, SIGNAL(hasBeenClicked()), &insideAirTemperature, SLOT(makeInvisible()));
    connect(&insideAirTemperature, SIGNAL(hasBeenClicked()), &outsideAirTemperature, SLOT(makeVisible()));
}

void EngineMonitor::setupStatusItem()
{
    statusItem.setPos(5, 300);
	graphicsScene.addItem(&statusItem);
    statusItem.setVisible(true);
}

void EngineMonitor::setupTimeToDestinationItem()
{
    timeToDestinationItem.setPos(0, 65);
	timeToDestinationItem.setDefaultTextColor(Qt::white);
    graphicsScene.addItem(&timeToDestinationItem);
}

void EngineMonitor::setupFuelManagement()
{
	fuelManagement.setPos(-495, -240);
	fuelManagement.setScale(1.8);
    fuelManagement.setVisible(false);
	connect(&fuelFlow, SIGNAL(hasBeenClicked()), &fuelManagement, SLOT(activateOverlay()));
	graphicsScene.addItem(&fuelManagement);
    fuelDisplay.setPos(102,102);
    graphicsScene.addItem(&fuelDisplay);
}

void EngineMonitor::setupManifoldPressure()
{
    manifoldPressure.setPos(100, 200);
    manifoldPressure.setStartSpan(230.0, 240.0);
    rpmIndicator.setBorders(0.0, 35.0);
    graphicsScene.addItem(&manifoldPressure);
    manifoldPressure.setVisible(true);
}

void EngineMonitor::setFuelData(double fuelFlowValue, double fuelAbsoluteValue)
{
	fuelFlow.setValue(fuelFlowValue);
	fuelManagement.setFuelFlow(fuelFlowValue);
    fuelDisplay.setFuelFlow(fuelFlowValue);
	fuelManagement.reduceFuelAmount(fuelAbsoluteValue);
}

void EngineMonitor::setTimeToDestination(double time)
{
	timeToDestinationItem.setPlainText(QString::number(time, 'f', 1).prepend("Time to destination: ").append(" minutes"));
	fuelManagement.setTimeToDestination(time);
    fuelDisplay.setTimeToDestination(time);
}

void EngineMonitor::userMessageHandler(QString title, QString content, bool endApplication)
{
	QMessageBox::warning(this, title, content);
	if(endApplication)
	{
		qApp->quit();
	}
}

void EngineMonitor::showStatusMessage(QString text, QColor color)
{
    //qDebug() << Q_FUNC_INFO;
	statusItem.setPlainText(text);
	statusItem.setDefaultTextColor(color);
}

void EngineMonitor::demoFunction()
{
	qsrand(QDateTime::currentDateTime().toTime_t());
    static double rpm = 1100.0;
    rpm += 5.0;

    rpmIndicator.setValue(rpm);
    manifoldPressure.setValue(10);

    static double basicEGT = 750.0;
    static bool egtUp = true;
	static bool leaned = false;
	static double off13 = 0.0;
	static double off24 = 0.0;
	if(leaned)
	{
        if(basicEGT < 680.0)
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
    chtEgt.setEgtValues(basicEGT+51.0+off13, basicEGT+10.0-off24, basicEGT+5.0-off13, basicEGT+30.0+off24);

    static double basicCHT = 60.0;

    if(basicCHT > 250.0)
	{
        basicCHT -= 0.5;
    } else {
        basicCHT += 0.5;
    }
    static double offset1 = double(qrand())/double(RAND_MAX)*50.0;
	static double offset2 = double(qrand())/double(RAND_MAX)*7.0;
	static double offset3 = double(qrand())/double(RAND_MAX)*15.0;
    static double offset4 = double(qrand())/double(RAND_MAX)*9.0;
    chtEgt.setChtValues(basicCHT+offset1,
                        basicCHT-offset2,
                        basicCHT+offset3,
                        basicCHT-offset4);

    static double oilTemp = 100.0;
    if(oilTemp < 80.0)
	{
        oilTemp = 100.0;
	}
    oilTemp += 0.25;

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

    static double amperes = 35.0;
	amperes -= 0.1;
    if(amperes < -20.0)
	{
        amperes = 30.0;
	}
	ampereMeter.setValue(amperes);

    static double flow = 7.0;
	flow -= 0.05;
	if(flow < 0.0)
	{
        flow = 7.0;
	}
	fuelFlow.setValue(flow);
	fuelManagement.setFuelFlow(flow);
	fuelManagement.reduceFuelAmount(flow*200.0/1000.0/60.0/60.0);
    fuelDisplay.setFuelFlow(flow);
    fuelDisplay.reduceFuelAmount(flow*200.0/1000.0/60.0/60.0);

	static double airTemp = -10.0;
	airTemp += 0.07;
	if(airTemp > 40.0)
	{
		airTemp = -10.0;
	}
	outsideAirTemperature.setValue(airTemp);
	insideAirTemperature.setValue(airTemp);

}

//void EngineMonitor::saveSceneToSvg(const QString fileName)
//{
//    QSvgGenerator generator;
//    generator.setFileName(fileName);
////	generator.setSize(QSize(800, 600));
////	generator.setViewBox(QRect(0, 0, 800, 600));
////	generator.setTitle(tr("SVG Generator Example Drawing"));
////	generator.setDescription(tr("An SVG drawing created by the SVG Generator"));
////	QPainter painter;
////	painter.begin(&generator);
////	graphicsScene.render(&painter);
////	painter.end();
//}

void EngineMonitor::realtimeDataSlot()
{
  static QTime time(QTime::currentTime());
  // calculate two new data points:
  double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.500 /*.002*/) // at most add point every 2 ms
  {
//    // add data to lines:
//    customPlot->graph(0)->addData(key, chtEgt.getCurrentChtValues().at(0));
//    customPlot->graph(1)->addData(key, chtEgt.getCurrentChtValues().at(1));
//    customPlot->graph(2)->addData(key, chtEgt.getCurrentChtValues().at(2));
//    customPlot->graph(3)->addData(key, chtEgt.getCurrentChtValues().at(3));
//    //customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
//    // rescale value (vertical) axis to fit the current data:
////    customPlot->graph(0)->rescaleValueAxis();
////    customPlot->graph(1)->rescaleValueAxis(true);
//    lastPointKey = key;

//    // make key axis range scroll with the data (at a constant range size of 8):
//    customPlot->xAxis->setRange(key, 120, Qt::AlignRight);
//    customPlot->replot();
  }

}

void EngineMonitor::processPendingDatagrams() {
//    QByteArray datagram;
//    datagram.resize(socket->pendingDatagramSize());
//    socket->readDatagram(datagram.data(), datagram.size());

//    qDebug()<<"Processing";
//    qDebug()<<"Message: " << tr("Received datagram: \"%1\"").arg(datagram.data());

    QString msg;
    //while (socket->hasPendingDatagrams()) {
    //    QByteArray buffer;
    //    buffer.resize(socket->pendingDatagramSize());
    //    socket->readDatagram(buffer.data(), buffer.size());
    //    msg.append(buffer.data());
    //    qDebug()<<buffer.data();
    //}

    qDebug()<< msg;
}

void EngineMonitor::onUpdateWindInfo(float spd, float dir, float mHdg) {
    windVector.updateWind(spd, dir, mHdg);
}

void EngineMonitor::setupWindVector() {
    windVector.setPos(50, 385);
    graphicsScene.addItem(&windVector);
    windVector.setVisible(false);
}

void EngineMonitor::connectSignals() {
    // Connect buttonBar to the alarm window for alarm acknowledgement
    connect(&buttonBar, SIGNAL(sendAlarmAck()), &ems_full, SLOT(onAckAlarm()));

    // Connect buttonBar to the fuelDisplay window to increment fuel amount
//    connect(&buttonBar, SIGNAL(sendFuelChange(QString)), &fuelDisplay, SLOT(onFuelAmountChange(QString)));

    // Connect signal for a flashing alarm to the button bar to be able to show the 'Ack' button
    connect(&ems_full, SIGNAL(alarmFlashing()), &buttonBar, SLOT(onAlarmFlash()));

    qDebug()<<"Connecting hobb/flight time Signals";
    // Connect a timer for handling hobbs/flight time
    connect(&clockTimer, SIGNAL(timeout()), &hobbs, SLOT(onTic()));

    connect(&clockTimer, SIGNAL(timeout()), this, SLOT(setEngineConds()));

    connect(&buttonBar, SIGNAL(switchScene(int)), this, SLOT(onSwitchScene(int)));

    connect(&settings_scene, SIGNAL(zeroCurrent()), this, SLOT(onZeroCurrent()));

    connect(this, SIGNAL(updateEngineValues(qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal, qreal)), &ems_full, SLOT(onEngineValuesUpdate(qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal, qreal)));
}

void EngineMonitor::setupuserSettings()
{
    uSettings.setPos(400,240);
    graphicsScene.addItem(&uSettings);
    uSettings.setVisible(false);
}

void EngineMonitor::setEngineConds() {
    if (oilPressure.getValue() > 10.0) {
        hobbs.setEngineOn(true);
    } else {
        hobbs.setEngineOn(false);
    }

    if (oilTemperature.getValue() < warmupTemp) {
        timeOilTBelowWarmup += 1;
        if (timeOilTAboveWarmup < 5 * 60) {
            timeOilTAboveWarmup = 0;
            rpmIndicator.isWarmup = true;
        }
    } else {
        timeOilTAboveWarmup += 1;
        timeOilTBelowWarmup = 0;
        rpmIndicator.isWarmup = false;
    }
}

void EngineMonitor::onSwitchScene(int scene) {
    switch (scene) {
        case 1: // EMS
            this->setScene(&ems_full);
            break;

        case 2: // Settings
            this->setScene(&settings_scene);
            break;
    }

    this->scene()->setSceneRect(0,0,800,480);
    buttonBar.setPos(0,this->scene()->height());
    this->scene()->addItem(&buttonBar);

    qDebug() << "Is EMS Scene Active?: " + QString::number(ems_full.isActive());
    qDebug() << "Is Settings Scene Active?: " + QString::number(settings_scene.isActive());

}

void EngineMonitor::onZeroCurrent() {
    emit zeroCurrent();
}
