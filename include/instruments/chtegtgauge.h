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

#ifndef CHTEGTGAUGE_H
#define CHTEGTGAUGE_H

#include <QtWidgets>
#include <gaugesettings.h>
#include <datasmoothing.h>

//! CHT EGT Gauge Class
/*!
 * This class creates a combined CHT/EGT gauge.
*/

class ChtEgt : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ChtEgt(QGraphicsObject * parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void setBorders(double minimum, double maximum, double yellowBorder, double redBorder, double minEgt, double maxEgt);
	void addBetweenValue(double value);
    void setChtValues(double val1, double val2, double val3, double val4);
    void setEgtValues(double val1, double val2, double val3, double val4);
    const QList<double> &getCurrentChtValues() {return currentChtValues;}
    const QList<double> &getCurrentEgtValues() {return currentEgtValues;}
    void setGaugeType(QString type);

private:
    double calculateLocalChtValue(double value) const;
    double calculateLocalEgtValue(double value) const;
    double minChtValue, maxChtValue;
    double greenYellowChtValue, yellowRedChtValue;
    QList<double> currentChtValues;
    double minEgtValue, maxEgtValue;
    double greenYellowEgtValue, yellowRedEgtValue;
    QList<double> currentEgtValues;
	QList<double> betweenValues;
    bool isAlarmedRed = false;
    bool isAlarmedYellow = false;
    bool flashState = false;
    int cylinderAlarm;
    bool isAcknowledged = false;

    QString chtGaugeType;
    GaugeSettings chtGauge;
    int numOfRanges;
    double startRange;
    double endRange;
    QColor color;
    double minChtLocal;
    double maxChtLocal;
    int j;

    double currentLocal;

    GaugeSettings egtGauge;

    int numOfCht = 2; //This needs to be made to be able to be configured by the user
    int numOfEgt = 0; //This needs to be made to be able to be configured by the user

    DataSmoothing smoothData;
    bool smooth = true;
signals:
    void sendAlarm(QString, QColor, bool);
    void cancelAlarm(QString);

public slots:
    void changeFlashState();
    void onAlarmAck();
};

#endif // CHTEGTGAUGE_H
