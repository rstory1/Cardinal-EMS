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

    //  Get the interface type, Arduino or RDAC
    sensorInterfaceType = settings.value("Sensors/interface", "arduino").toString();

    setupLogFile();

    qDebug()<<"Enter connectSignals(): enginemonitor.cpp";
    connectSignals();
    qDebug() << "Returned from connectSignals(): enginemonitor.cpp";

    #ifndef USEDATABASE
        //Create the RDAC connector
        rdac.moveToThread(&rdacWorkerThread);

        sensorConvert.moveToThread(&sensorConvertWorkerThread);

        connect(&sensorConvert, SIGNAL(updateMonitor(qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal, qreal, qreal)), this, SLOT(setValuesBulkUpdate(qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal, qreal)));
        connect(this, SIGNAL(sendSerialData(QByteArray)), &rdac, SLOT(writeData(QByteArray)));
        connect(&rdac, SIGNAL(rdacUpdateMessage(qreal, qreal, quint16, quint16, quint16, quint16, quint16, quint16, quint16, quint16, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, quint16, qreal, qreal, qreal, quint16, qreal)), &sensorConvert, SLOT(onRdacUpdate(qreal, qreal, quint16, quint16, quint16, quint16, quint16, quint16, quint16, quint16, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, quint16, qreal, qreal, qreal, quint16, qreal)));
        connect(&rdac, SIGNAL(statusMessage(QString,QColor)), this, SLOT(showStatusMessage(QString,QColor)));
        connect(this, SIGNAL(zeroCurrent()), &sensorConvert, SLOT(onZeroCurrent()));
        connect(this, SIGNAL(startRdacConnect()), &rdac, SLOT(openSerialPort()));

        sensorConvertWorkerThread.start();
        rdacWorkerThread.start();
        emit startRdacConnect();
    #endif

}

EngineMonitor::~EngineMonitor()
{
    logFile->close();
#ifndef USEDATABASE
    sensorConvertWorkerThread.quit();
    sensorConvertWorkerThread.wait();
    rdacWorkerThread.quit();
    rdacWorkerThread.wait();
#endif
}

void EngineMonitor::setupLogFile()
{
    QDir dir(QApplication::applicationDirPath() + "/ems/engineLogs");
    if (!dir.exists())
        dir.mkpath(".");
    logFile = new QFile(QString(QApplication::applicationDirPath() + "/ems/engineLogs/EngineData ").append(QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh.mm")).append(".csv"), this);
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
    logFile->write("INDEX;TIME;EGT1;EGT2;EGT3;EGT4;CHT1;CHT2;CHT3;CHT4;OILT;OILP;OAT;IAT;BAT;CUR1;CUR2;RPM;MAP;FF;FUELP;HOBBS;FLIGHT\r\n");
}

void EngineMonitor::writeLogFile()
{
    static quint64 sample = 0;
    logFile->write(QString::number(sample).append(';').toLatin1());
    logFile->write(QDateTime::currentDateTimeUtc().toString("yyyy-dd-MM hh:mm:ss").append(';').toLatin1());
    QList<double> egtValues = ems_full.chtEgt.getCurrentEgtValues();
    logFile->write(QString::number(egtValues.value(0, 0.0), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(egtValues.value(1, 0.0), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(egtValues.value(2, 0.0), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(egtValues.value(3, 0.0), 'f', 0).append(';').toLatin1());
    QList<double> chtValues = ems_full.chtEgt.getCurrentChtValues();
    logFile->write(QString::number(chtValues.value(0, 0.0), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(chtValues.value(1, 0.0), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(chtValues.value(2, 0.0), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(chtValues.value(3, 0.0), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(ems_full.oilTemperature.getValue(), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(ems_full.oilPressure.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(ems_full.outsideAirTemperature.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(ems_full.insideAirTemperature.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(ems_full.voltMeter.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(ems_full.ampereMeter.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(ems_full.ampereMeter2.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(ems_full.rpmIndicator.getValue(), 'f', 0).append(';').toLatin1());
    logFile->write(QString::number(ems_full.manifoldPressure.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(ems_full.fuelFlow.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString::number(ems_full.fuelPressure.getValue(), 'f', 1).append(';').toLatin1());
    logFile->write(QString(ems_full.hobbs.getHobbsTime().append(';')).toLatin1());
    logFile->write(QString(ems_full.hobbs.getFlightTime().append(';')).toLatin1());
    logFile->write("\r\n");
    logFile->flush();
    ++sample;
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

    //QString msg;
    //while (socket->hasPendingDatagrams()) {
    //    QByteArray buffer;
    //    buffer.resize(socket->pendingDatagramSize());
    //    socket->readDatagram(buffer.data(), buffer.size());
    //    msg.append(buffer.data());
    //    qDebug()<<buffer.data();
    //}

    //qDebug()<< msg;
}

//void EngineMonitor::onUpdateWindInfo(float spd, float dir, float mHdg) {
//    windVector.updateWind(spd, dir, mHdg);
//}

//void EngineMonitor::setupWindVector() {
//    windVector.setPos(50, 385);
//    graphicsScene.addItem(&windVector);
//    windVector.setVisible(false);
//}

void EngineMonitor::connectSignals() {
    // Connect buttonBar to the alarm window for alarm acknowledgement
    connect(&buttonBar, SIGNAL(sendAlarmAck()), &ems_full, SLOT(onAckAlarm()));

    // Connect buttonBar to the fuelDisplay window to increment fuel amount
//    connect(&buttonBar, SIGNAL(sendFuelChange(QString)), &fuelDisplay, SLOT(onFuelAmountChange(QString)));

    // Connect signal for a flashing alarm to the button bar to be able to show the 'Ack' button
    connect(&ems_full, SIGNAL(alarmFlashing()), &buttonBar, SLOT(onAlarmFlash()));

    //qDebug()<<"Connecting hobb/flight time Signals";
    // Connect a timer for handling hobbs/flight time
    //connect(&clockTimer, SIGNAL(timeout()), &hobbs, SLOT(onTic()));

    //connect(&clockTimer, SIGNAL(timeout()), this, SLOT(setEngineConds()));

    connect(&buttonBar, SIGNAL(switchScene(int)), this, SLOT(onSwitchScene(int)));

    //connect(&settings_scene, SIGNAL(zeroCurrent()), this, SLOT(onZeroCurrent()));

    connect(&ems_full, SIGNAL(switchScene(int)), this, SLOT(onSwitchScene(int)));
    connect(&settings_scene, SIGNAL(switchScene(int)), this, SLOT(onSwitchScene(int)));

    connect(&settings_scene, SIGNAL(hobbsUpdated()), &ems_full.hobbs, SLOT(onHobbsINIChanged()));

    connect(this, SIGNAL(updateEngineValues(qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal)), &ems_full, SLOT(onEngineValuesUpdate(qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal,qreal)));

    connect(&settings_scene, SIGNAL(fuelUpdated()), &ems_full.fuelDisplay, SLOT(onFuelAmountChange()));

    connect(&ems_full, SIGNAL(sendSerialData(QByteArray)), this, SLOT(onSendSerialData(QByteArray)));

#ifndef USEDATABASE
    connect(&ems_full, SIGNAL(sendTimeData(qreal, QString)), &sensorConvert, SLOT(onUpdateFlightTime(qreal, QString)));
#endif

}

void EngineMonitor::setupuserSettings()
{
    uSettings.setPos(400,240);
    graphicsScene.addItem(&uSettings);
    uSettings.setVisible(false);
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
//    this->scene()->addItem(&buttonBar);

//    qDebug() << "Is EMS Scene Active?: " + QString::number(ems_full.isActive());
//    qDebug() << "Is Settings Scene Active?: " + QString::number(settings_scene.isActive());

}
