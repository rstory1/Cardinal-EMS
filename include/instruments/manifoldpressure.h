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

#ifndef MANIFOLDPRESSURE_H
#define MANIFOLDPRESSURE_H

#include <QtWidgets>

//! ManifoldPressure Class
/*!
 * This class creates a circular manifold pressure gauge
*/

class ManifoldPressure : public QGraphicsItem
{
public:
	ManifoldPressure(QGraphicsItem * parent = 0);
	~ManifoldPressure();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setStartSpan(double start, double span);
	void setBorders(double minimum, double maximum, double greenBorder, double redBorder);
	void addBetweenValue(double value);
    void setValue(qreal value, qreal rawValue);
    qreal getValue() {return currentValue;};
    qreal getRawValue() {return currentRawValue;};
private:
	double calculateLocalValue(double value) const;
    qreal minValue, maxValue, currentValue, currentRawValue;
	double whiteGreenBorder, greenRedBorder;
	double startAngle, spanAngle;
	QList<double> beetweenValues;
    QList<double> valueHistory;
    QList<double> rawValueHistory;
    bool showRawValue = false;
    QString pressure;
};


#endif // MANIFOLDPRESSURE_H
