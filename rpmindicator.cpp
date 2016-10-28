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
  , startAngle(0.0)
  , spanAngle(0.0)
{
}

RpmIndicator::~RpmIndicator()
{
}

QRectF RpmIndicator::boundingRect() const
{
	return QRectF(-200.0, -140.0, 400.0, 280.0);
}

void RpmIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//Draw the arc
	QRectF circle = QRectF(-130.0, -130.0, 260.0, 260.0);
	//Calculate angles for white and red arc part
	double whiteGreenAngle = calculateLocalValue(whiteGreenBorder);
	double greenRedAngle = calculateLocalValue(greenRedBorder);
	//Draw the green basis
	painter->setPen(QPen(Qt::green, 0));
	painter->setBrush(Qt::green);
	painter->drawPie(circle, startAngle*16.0, -spanAngle*16.0);
	//Draw the green part
	painter->setPen(QPen(Qt::white, 0));
	painter->setBrush(Qt::white);
	painter->drawPie(circle, startAngle*16.0, -fabs(whiteGreenAngle-startAngle)*16.0);
	//Draw the red part
	painter->setPen(QPen(Qt::red, 0));
	painter->setBrush(Qt::red);
	painter->drawPie(circle, greenRedAngle*16.0, -fabs(startAngle-spanAngle-greenRedAngle)*16.0);
	//Overlay the center with a black circle
	painter->setPen(QPen(Qt::black, 0));
	painter->setBrush(Qt::black);
	painter->drawEllipse(circle.center(), 110.0, 110.0);

	//Set the pen and font to draw the ticks
	painter->setPen(QPen(Qt::white, 2));
	painter->setFont(QFont("Arial", 14));
	foreach(double value, beetweenValues)
	{
		//Rotate painter and draw the ticks
		painter->save();
		painter->rotate(-calculateLocalValue(value));
		painter->drawLine(100, 0, 130, 0);
		painter->restore();
		//Define a box, move it and draw the text centered to this position
		QRectF textRect(-10, -10, 20, 20);
		textRect.moveCenter(QPointF(cos(calculateLocalValue(value)/180.0*M_PI)*85.0, -sin(calculateLocalValue(value)/180.0*M_PI)*85.0));
		painter->drawText(textRect, Qt::AlignCenter, QString::number(value/100.0, 'f', 0));
	}

	//Draw the center text
	QRectF centerTextRect(-50, -50, 100, 100);
	painter->drawText(centerTextRect, Qt::AlignCenter, "x 100 rpm");

	//Draw the needle if value is in range
	if((currentValue > minValue) &&
			(currentValue < maxValue))
	{
		//Needle is white with 1px black border
		painter->setPen(QPen(Qt::black, 1));
		painter->setBrush(Qt::white);
		//Define the shape
		QPolygonF marker;
		marker.append(QPointF(110.0, 0.0));
		marker.append(QPointF(140.0, -7.0));
		marker.append(QPointF(140.0, 7.0));
		//Rotate the painter and draw the needle
		painter->save();
		painter->rotate(-calculateLocalValue(currentValue));
		painter->drawPolygon(marker);
		painter->restore();
	}

	//If number is in red range, draw it red
	if(currentValue > greenRedBorder)
	{
		painter->setPen(Qt::red);
	}
	else
	{
		painter->setPen(Qt::white);
	}
	//Round value to the nearest 10
	QString rpm = QString::number(currentValue-fmod(currentValue, 10.0), 'f', 0);

	//Set position and font for the value and draw it
	QRectF textRect(-100, 35, 170, 65);
	painter->setFont(QFont("Arial", 30, 1));
	painter->drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, rpm);
	//Set position and font for the unit and draw it
	QRectF unitRect(90, 35, 100, 65);
	painter->setFont(QFont("Arial", 20, 1));
    painter->drawText(unitRect, Qt::AlignLeft | Qt::AlignVCenter, "RPM");
}

void RpmIndicator::setStartSpan(double start, double span)
{
	startAngle = start;
	spanAngle = span;
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
	return startAngle-((value-minValue)/(maxValue-minValue)*spanAngle);
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
