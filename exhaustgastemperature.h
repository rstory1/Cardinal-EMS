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

#ifndef EXHAUSTGASTEMPERATURE_H
#define EXHAUSTGASTEMPERATURE_H

#include <QtWidgets>

//! ExhaustGasTemperature Class
/*!
 * This class creates a stand alone EGT gauge
*/

class ExhaustGasTemperature : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ExhaustGasTemperature(QGraphicsObject * parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
	void setBorders(double minimum, double maximum, double yellowBorder, double redBorder);
	void addBetweenValue(double value);
	void setValues(double val1, double val2, double val3, double val4);
	void setLeanWindow(double value);
	bool isLeanAssistActive() const;
	const QList<double> &getCurrentValues() {return currentValues;};
private:
	double calculateLocalValue(double value) const;
	double minValue, maxValue;
	double greenYellowValue, yellowRedValue;
	double leanWindow, leanMinValue;
	bool leanAssistActive;
	QList<double> peakValues;
	QList<bool> peakFound;
	QList<quint8> peakOrder;
	QList<double> currentValues;
	QList<double> betweenValues;
    bool isAlarmedRed = false;
    bool isAlarmedYellow = false;
    bool flashState;
    int cylinderAlarm;
signals:
    void sendAlarm(QString, QColor, bool);
    void cancelAlarm(QString);
public slots:
    void changeFlashState();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *)
	{
		leanAssistActive = !leanAssistActive;
		if(leanAssistActive)
		{
			QList<double> egts = currentValues;
			qSort(egts.begin(), egts.end());
			leanMinValue = qMax(egts.first() - 50.0, minValue);
			if(egts.last() < minValue)
			{
				leanAssistActive = false;
				return;
			}
			peakValues = currentValues;
			peakFound.clear();
			peakFound << false << false << false << false;
			peakOrder.clear();
			peakOrder << 0 << 0 << 0 << 0;
		}
		update();
	}
};

#endif // EXHAUSTGASTEMPERATURE_H
