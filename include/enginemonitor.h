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

#define USEDATABASE
#ifdef USEDATABASE
#include "sensorconvert.h"
#include "rdacconnect.h"
#endif

#include "alarmBox.h"
#include <buttonbar.h>
#include <qcustomplot.h>
#include <udpsocket.h>
#include <scenes/settings_scene.h>
#include <scenes/emsfull.h>

#include <QMessageBox>

//! Engine Monitor Class
/*!
 * This class is the main class and handles the overall function of the app. The grpahics scene is setup here and multiple signal/slots are connected here as well.
*/

class EngineMonitor : public QGraphicsView
{
	Q_OBJECT
public:
	EngineMonitor(QWidget *parent = 0);
	~EngineMonitor();
private:
    void setupAlarm();
	void setupRpmIndicator();
	void setupBarGraphs();
	void setupExhaustGasTemperature();
	void setupCylinderHeadTemperature();
	void setupStatusItem();
	void setupTimeToDestinationItem();
	void setupFuelManagement();
	void setupManifoldPressure();
	void setupLogFile();
    void setupChtEgt();
    void setupWindVector();
    //void saveSceneToSvg(const QString fileName = "./out/output.svg");
    void handleAlarm(int alarmColor, int alarmSeverity, QString alarmText, QString alarmGauge);
    void cancelAlarm(QString alarmGauge);
    void connectSignals();
    void setupHourMeter();
    void setupuserSettings();

    QFile *logFile;
    QGraphicsScene graphicsScene;
    QGraphicsTextItem statusItem;
    QSettings settings;
    QSettings gaugeSettings;
    QString sensorInterfaceType;
    QTimer flashTimer;
    ButtonBar buttonBar;
    //QCustomPlot *customPlot;
    //QCPGraph *graphic;
    QTimer clockTimer;
    userSettings uSettings;
    int timeOilTAboveWarmup=0;
    int timeOilTBelowWarmup=0;

    settingsScene settings_scene;
    emsFull ems_full;

    QString currentScene = "";

#ifdef USEDATABASE
    QThread dbWorkerThread;
    RDACconnect rdac;
    SensorConvert sensorConvert;
    DatabaseHandler dbHandler;
#endif

#ifndef USEDATABASE
    QThread sensorThread;
    SensorConvert sensorConvert;
#endif

private slots:
    void writeLogFile();
    void realtimeDataSlot();

public slots:
	void userMessageHandler(QString title, QString content, bool endApplication);
    void showStatusMessage(QString text, QColor color);
    void setValuesBulkUpdate(qreal rpm, qreal fuelFlow, qreal oilTemp, qreal oilPress, qreal amps, qreal amps2, qreal volts, qreal egt1, qreal egt2, qreal egt3, qreal egt4, qreal cht1, qreal cht2, qreal cht3, qreal cht4, qreal oat, qreal iat, qreal map, qreal fuelPress) {
        emit updateEngineValues(rpm, fuelFlow, oilTemp, oilPress, amps, amps2, volts, egt1, egt2, egt3, egt4, cht1, cht2, cht3, cht4, oat, iat, map, fuelPress);
    }
    void processPendingDatagrams();
    void onSwitchScene(int scene);
    void onSendSerialData(QByteArray emsSerial) { emit sendSerialData(emsSerial); };

signals:
    void updateEngineValues(qreal rpm, qreal fuelFlow, qreal oilTemp, qreal oilPress, qreal amps, qreal amps2, qreal volts, qreal egt1, qreal egt2, qreal egt3, qreal egt4, qreal cht1, qreal cht2, qreal cht3, qreal cht4, qreal oat, qreal iat, qreal map, qreal fuelPress);
    void sendSerialData(QByteArray emsSerial);
};

#endif // ENGINEMONITOR_H
