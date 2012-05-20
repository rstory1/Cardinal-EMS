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

#include "cylinderheadtemperature.h"

CylinderHeadTemperature::CylinderHeadTemperature(QGraphicsItem *parent) : QGraphicsItem(parent)
  , minValue(0.0)
  , maxValue(0.0)
  , greenYellowValue(0.0)
  , yellowRedValue(0.0)
{
	currentValues << 0.0 << 0.0 << 0.0 << 0.0;
}

QRectF CylinderHeadTemperature::boundingRect() const
{
	return QRectF(-125, -170, 250, 300);
}

void CylinderHeadTemperature::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//Draw the side legend
	painter->setPen(QPen(Qt::transparent, 0));
	painter->setBrush(Qt::green);
	painter->drawRect(QRectF(QPointF(50.0, calculateLocalValue(minValue)), QPointF(80.0, calculateLocalValue(greenYellowValue))));
	painter->setBrush(Qt::yellow);
	painter->drawRect(QRectF(QPointF(50.0, calculateLocalValue(greenYellowValue)), QPointF(80.0, calculateLocalValue(yellowRedValue))));
	painter->setBrush(Qt::red);
	painter->drawRect(QRectF(QPointF(50.0, calculateLocalValue(yellowRedValue)), QPointF(80.0, calculateLocalValue(maxValue))));

	//Set painter for texts
	painter->setPen(QPen(Qt::white, 1));
	painter->setFont(QFont("Arial", 12));

	//Draw the static texts
	painter->drawText(QRectF(90.0, -162.5, 35.0, 20.0), Qt::AlignRight | Qt::AlignVCenter, "CHT");
	painter->drawText(QRectF(90.0, 105.0, 35.0, 20.0), Qt::AlignRight | Qt::AlignVCenter, "°C");

	//Draw the ticks and numbers
	foreach(double value, betweenValues)
	{
		painter->drawLine(80, calculateLocalValue(value), 90, calculateLocalValue(value));
		painter->drawText(QRectF(90.0, calculateLocalValue(value)-10.0, 35.0, 20.0), Qt::AlignRight | Qt::AlignVCenter, QString::number(value, 'f', 0));
	}

	//Draw the red line to define warning range
	painter->setPen(Qt::red);
	painter->drawLine(-125, calculateLocalValue(yellowRedValue), 35, calculateLocalValue(yellowRedValue));

	//Draw center dashed lines
	painter->setPen(QPen(Qt::white, 1, Qt::DashLine));
	for(int i = 0; i < 4; i++)
	{
		painter->drawLine(i*40-105, -125, i*40-105, 125);
	}

	//Draw the bar graphes
	for(int i = 0; i < 4; i++)
	{
		QRectF barRect = QRectF(QPointF(i*40-120, 125), QPointF(i*40-90, calculateLocalValue(currentValues.value(i))));
		if(currentValues.at(i) > yellowRedValue)
		{
			//If value is in warning area, bar is drawn red
			painter->setBrush(Qt::red);
		}
		else if(currentValues.at(i) > greenYellowValue)
		{
			//If value is in caution area, bar is drawn yellow
			painter->setBrush(Qt::yellow);
		}
		else
		{
			//In all other cases, bar is drawn green
			painter->setBrush(Qt::green);
		}
		painter->setPen(QPen(Qt::transparent, 0));
		if((currentValues.at(i) > minValue) &&
				(currentValues.at(i) < maxValue))
		{
			painter->drawRect(barRect);
		}
		else
		{
			//If value is outside the displayed range, draw a red cross
			painter->setPen(QPen(Qt::red, 2));
			painter->drawLine(barRect.left(), 125, barRect.right(), -125);
			painter->drawLine(barRect.left(), -125, barRect.right(), 125);
		}
		painter->setPen(painter->brush().color());
		if(painter->brush().color() == Qt::green)
		{
			//If value is in normal range, draw text in white
			painter->setPen(Qt::white);
		}
		//Define text position and move to current column
		QRectF textRect(-30, -20, 60, 40);
		textRect.moveCenter(QPointF(i*40-105, -140));
		if(i%2)
		{
			//All odd values should be raised
			textRect.translate(QPointF(0.0, -20.0));
		}
		//Draw the readout
		painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i), 'f', 0));
	}
}

double CylinderHeadTemperature::calculateLocalValue(double value) const
{
	return -(value-minValue)/(maxValue-minValue)*250.0+125.0;
}

void CylinderHeadTemperature::addBetweenValue(double value)
{
	betweenValues.append(value);
}

void CylinderHeadTemperature::setValues(double val1, double val2, double val3, double val4)
{
	currentValues.replace(0, val1);
	currentValues.replace(1, val2);
	currentValues.replace(2, val3);
	currentValues.replace(3, val4);
	update();
}

void CylinderHeadTemperature::setBorders(double minimum, double maximum, double yellowBorder, double redBorder)
{
	minValue = minimum;
	maxValue = maximum;
	greenYellowValue = yellowBorder;
	yellowRedValue = redBorder;
}
