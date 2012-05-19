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

#include "rpmindicator.h"

RpmIndicator::RpmIndicator(QGraphicsItem *parent) : QGraphicsItem(parent)
  , minValue(0.0)
  , maxValue(0.0)
  , currentValue(0.0)
  , whiteGreenBorder(0.0)
  , greenRedBorder(0.0)
{
}

RpmIndicator::~RpmIndicator()
{
}

QRectF RpmIndicator::boundingRect() const
{
	return QRectF(-200.0, -160.0, 400.0, 310.0);
}

void RpmIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//Draw the arc
	QRectF circle = QRectF(-150.0, -150.0, 300.0, 300.0);
	int minAngle = -230.0;
	int whiteGreenAngle = calculateLocalValue(whiteGreenBorder);
	int greenRedAngle = calculateLocalValue(greenRedBorder);
	int maxAngle = 10.0;
	painter->setPen(QPen(Qt::transparent, 0));
	painter->setBrush(Qt::green);
	painter->drawPie(circle, 230*16.0, -240.0*16.0);
	painter->setBrush(Qt::white);
	painter->drawPie(circle, -minAngle*16.0, -fabs(whiteGreenAngle-minAngle)*16.0);
	painter->setBrush(Qt::red);
	painter->drawPie(circle, -greenRedAngle*16.0, -fabs(greenRedAngle-maxAngle)*16.0);
	painter->setBrush(Qt::black);
	painter->drawEllipse(circle.center(), 130.0, 130.0);

	//Draw the markers
	QPen markerPen(Qt::white, 2);
	painter->setPen(markerPen);
	painter->setFont(QFont("Arial", 14));
	foreach(double value, beetweenValues)
	{
		painter->save();
		painter->rotate(calculateLocalValue(value));
		painter->drawLine(120, 0, 150, 0);
		painter->restore();
		QRectF textRect(-10, -10, 20, 20);
		textRect.moveCenter(QPointF(cos(calculateLocalValue(value)/180.0*M_PI)*110.0, sin(calculateLocalValue(value)/180.0*M_PI)*110.0));
		painter->drawText(textRect, Qt::AlignCenter, QString::number(value/100.0, 'f', 0));
	}

	//Draw the center text
	QRectF centerTextRect(-50, -50, 100, 100);
	painter->drawText(centerTextRect, Qt::AlignCenter, "x 100 rpm");

	//Draw the needle
	if((currentValue > minValue) &&
			(currentValue < maxValue))
	{
	painter->setPen(Qt::black);
	painter->setBrush(Qt::white);
	QPolygonF marker;
	marker.append(QPointF(130.0, 0.0));
	marker.append(QPointF(160.0, -7.0));
	marker.append(QPointF(160.0, 7.0));
	painter->save();
	painter->rotate(calculateLocalValue(currentValue));
	painter->drawPolygon(marker);
	painter->restore();
	}

	//Draw the readout
	QRectF textRect(-100, 35, 170, 65);
	if(currentValue > greenRedBorder)
	{
		painter->setPen(Qt::red);
	}
	else
	{
		painter->setPen(Qt::white);
	}
	QString rpm = QString::number(currentValue-fmod(currentValue, 10.0), 'f', 0);
	if(rpm.size() > 3)
	{
		rpm.insert(1, '\'');
	}
	painter->setFont(QFont("Arial", 30, 1));
	painter->drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, rpm);
	QRectF unitRect(90, 35, 100, 65);
	painter->setFont(QFont("Arial", 20, 1));
	painter->drawText(unitRect, Qt::AlignLeft | Qt::AlignVCenter, "rpm");
}

void RpmIndicator::setBorders(double minimum, double maximum, double greenBorder, double redBorder)
{
	minValue = minimum;
	maxValue = maximum;
	whiteGreenBorder = greenBorder;
	greenRedBorder = redBorder;
}

double RpmIndicator::calculateLocalValue(double value) const
{
	return ((value-maxValue)/(maxValue-minValue)*240.0)+10.0;
}

void RpmIndicator::addBetweenValue(double value)
{
	beetweenValues.append(value);
}

void RpmIndicator::setValue(double value)
{
	currentValue = value;
	update();
}
