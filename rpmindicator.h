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
    void setBorders(double minimum, double maximum, double whiteGreen, double greenRed,double yellowRed, double greenYellow, double redYellow, double yellowGreen,double yellowRedWarmup, double greenYellowWarmup, double redYellowWarmup, double yellowGreenWarmup);
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
    QSettings gaugeSettings;
    void paintWarmup(QPainter parentPainter);
    void paintNormal(QPainter parentPainter);
    bool flashState;

signals:
    void sendAlarm(QString, QColor, bool);
    void cancelAlarm(QString);

public slots:
    void changeFlashState();

};

#endif // RPMINDICATOR_H
