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

#include <QtGui>

class RpmIndicator : public QGraphicsItem
{
public:
	RpmIndicator(QGraphicsItem * parent = 0);
	~RpmIndicator();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setBorders(double minimum, double maximum, double greenBorder, double redBorder);
	void addBetweenValue(double value);
	void setValue(double value);
private:
	double calculateLocalValue(double value) const;
	double minValue, maxValue, currentValue;
	double whiteGreenBorder, greenRedBorder;
	double startAngle, spanAngle;
	QList<double> beetweenValues;
};

#endif // RPMINDICATOR_H
