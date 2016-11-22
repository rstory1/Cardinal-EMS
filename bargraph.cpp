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

#include "bargraph.h"

BarGraph::BarGraph(QGraphicsObject *parent)
	: QGraphicsObject(parent)
	, titleText("HEAD")
	, unitText("UNIT")
	, minValue(0.0)
	, maxValue(0.0)
	, currentValue(0.0)
	, barPrecision(0)
	, readoutPrecision(0)
{
}

QRectF BarGraph::boundingRect() const
{
	return QRectF(-20, -70, 40, 140);
}

void BarGraph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//Save thje painter and deactivate Antialising for rectangle drawing
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, false);
	painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

	//Draw ticks with values
	painter->setPen(Qt::white);
	foreach(double value, beetweenValues)
	{
		painter->drawLine(QPointF(-20.0, calculateLocalValue(value)), QPointF(-10.0, calculateLocalValue(value)));
		painter->drawText(QRectF(-50, calculateLocalValue(value)-10.0, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(value, 'f', barPrecision));
	}

	//Define pen, brush and rect for the bar
	painter->setPen(Qt::green);
	painter->setBrush(Qt::green);
	painter->drawRect(QRectF(QPointF(-10.0, -50.0), QPointF(10.0, 50.0)));
	if(!colorStops.isEmpty())
	{
		//If there are color stops, go through them
		foreach(ColorStop colorStop, colorStops)
		{
			//Set pen and brish to color and draw the bar
			painter->setPen(colorStop.color);
			painter->setBrush(colorStop.color);
			painter->drawRect(QRectF(QPointF(-10.0, calculateLocalValue(qMin(qMax(colorStop.minValue, minValue), maxValue))), QPointF(10.0, calculateLocalValue(qMax(qMin(colorStop.maxValue, maxValue), minValue)))));
		}
	}

	//Restore the painter with antialising
	painter->restore();

	//Draw Texts around (title, min and max value)
	painter->setPen(Qt::white);
	painter->drawText(QRectF(-20, -70, 40, 20), Qt::AlignCenter, titleText);
	painter->drawText(QRectF(-50, -60, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(maxValue, 'f', barPrecision));
	painter->drawText(QRectF(-50, 40, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(minValue, 'f', barPrecision));

	//Draw readout
	painter->drawText(QRectF(-20, 50, 40, 20), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));
	painter->drawText(QRectF(10, 50, 20, 20), Qt::AlignCenter, unitText);

	//Draw marker
	if((currentValue>minValue) && (currentValue<maxValue))
	{
		painter->setPen(Qt::black);
		painter->setBrush(Qt::white);
		QPolygonF marker;
		marker.append(QPointF(-10.0, 0.0));
		marker.append(QPointF(20.0, -7.0));
		marker.append(QPointF(20.0, 7.0));
		painter->translate(QPointF(0.0, calculateLocalValue(currentValue)));
		painter->drawPolygon(marker);
	}
}

void BarGraph::setTitle(QString title)
{
	titleText = title;
}

void BarGraph::setUnit(QString unit)
{
	unitText = unit;
}

void BarGraph::setBorders(double minimum, double maximum)
{
	minValue = minimum;
	maxValue = maximum;
}

void BarGraph::setPrecision(quint8 readout, quint8 bar)
{
	readoutPrecision = readout;
	barPrecision = bar;
}

void BarGraph::addBetweenValue(double value)
{
	beetweenValues.append(value);
}

double BarGraph::calculateLocalValue(double value) const
{
	return -(value-minValue)/(maxValue-minValue)*100.0+50.0;
}

void BarGraph::setValue(double value)
{
	currentValue = value;
	update();
}

void BarGraph::addColorStop(ColorStop stop)
{
	colorStops.append(stop);
}
