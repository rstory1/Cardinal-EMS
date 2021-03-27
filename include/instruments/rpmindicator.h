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

#ifndef RPMINDICATOR_H
#define RPMINDICATOR_H

#include <QtWidgets>
#include <QtCore>
#include <alarmBox.h>
#include <gaugesettings.h>

//! RPM Indicator Class
/*!
 * This class creates an RPM gauge
*/

class RpmIndicator : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit RpmIndicator(QGraphicsObject* parent = 0);
    //~RpmIndicator();
    QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setStartSpan(double start, double span);
    void setBorders(double minimum, double maximum);
	void addBetweenValue(double value);
	void setValue(double value);
    double getValue() {return currentValue;};
    bool isWarmup;
    bool isAlarmedRed = false;
    bool isAlarmedYellow = false;
private:
	double calculateLocalValue(double value) const;
	double minValue, maxValue, currentValue;
    double whiteGreenBorder, greenRedBorder, yellowRedBorder, greenYellowBorder, redYellowBorder, yellowGreenBorder;
    double yellowRedBorderWarmup, greenYellowBorderWarmup, redYellowBorderWarmup, yellowGreenBorderWarmup;
    double startAngle, spanAngle;
    QList<double> beetweenValues;
    bool flashState = false;

    GaugeSettings gauge;

    int i;
    float startRange;
    float endRange;
    float startVal;
    float endVal;
    QColor color;
    int numOfRanges;

    bool isAcknowledged;

    QTimer updateTimer;

    bool dataIsValid = false;

signals:
    void sendAlarm(QString, QColor, bool);
    void cancelAlarm(QString);

public slots:
    void changeFlashState();
    void onAlarmAck() {
        isAcknowledged = true;
    }

private slots:
    void onUpdate() {update();}

};

#endif // RPMINDICATOR_H
