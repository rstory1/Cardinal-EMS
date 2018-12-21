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
#include "bargraph.h"
#include "fuelmanagement.h"
#include "manifoldpressure.h"
#include "alarmBox.h"
#include "textBoxGauge.h"
#include "fueldisplay.h"
#include "chtegtgauge.h"
#include <buttonbar.h>
#include <qcustomplot.h>
#include <udpsocket.h>
#include <windvector.h>
#include <hourmeter.h>
#include <usersettings.h>

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

	QGraphicsScene graphicsScene;
    RpmIndicator rpmIndicator;
	BarGraph oilTemperature;
	BarGraph oilPressure;
	BarGraph voltMeter;
	BarGraph ampereMeter;
	BarGraph fuelFlow;
    TextBox insideAirTemperature;
    TextBox outsideAirTemperature;
	QGraphicsTextItem statusItem;
	QGraphicsTextItem timeToDestinationItem;
	FuelManagement fuelManagement;
    FuelDisplay fuelDisplay;
	ManifoldPressure manifoldPressure;
	QFile *logFile;
    QSettings settings;
    QSettings gaugeSettings;
    QString sensorInterfaceType;
    AlarmBox alarmWindow;
    int warmupTemp;
    QTimer flashTimer;
    ChtEgt chtEgt;
    ButtonBar buttonBar;
    QCustomPlot *customPlot;
    QCPGraph *graphic;
    QTimer dataTimer;
    //QUdpSocket *socket;
    WindVector windVector;
    QTimer clockTimer;
    HourMeter hobbs;
    userSettings uSettings;
    int timeOilTAboveWarmup=0;
    int timeOilTBelowWarmup=0;

private slots:
	void demoFunction();
    void writeLogFile();
    void realtimeDataSlot();
    void setEngineConds();

public slots:
	void setTimeToDestination(double time);
	void userMessageHandler(QString title, QString content, bool endApplication);
    void showStatusMessage(QString text, QColor color);
    void setValuesBulkUpdate(qreal rpm, qreal fuelFlow, qreal oilTemp, qreal oilPress, qreal amps, qreal volts, qreal egt1, qreal egt2, qreal egt3, qreal egt4, qreal cht1, qreal cht2, qreal cht3, qreal cht4, qreal oat, qreal iat);
    void setFuelData(double fuelFlowValue, double fuelAbsoluteValue);
    void processPendingDatagrams();
    void onUpdateWindInfo(float spd, float dir, float mHdg);

};

#endif // ENGINEMONITOR_H
