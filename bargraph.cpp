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
    return QRectF(-45, -75, 85, 160);
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
//	painter->setPen(Qt::white);
//	foreach(double value, beetweenValues)
//	{
//		painter->drawLine(QPointF(-20.0, calculateLocalValue(value)), QPointF(-10.0, calculateLocalValue(value)));
//		painter->drawText(QRectF(-50, calculateLocalValue(value)-10.0, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(value, 'f', barPrecision));
//	}

	//Define pen, brush and rect for the bar
	painter->setPen(Qt::green);
	painter->setBrush(Qt::green);
    if (horizontal) {
        painter->drawRect(QRectF(QPointF(-50.0, -10.0), QPointF(50.0, 10.0)));
    } else {
        painter->drawRect(QRectF(QPointF(-10.0, -50.0), QPointF(10.0, 50.0)));

        //Draw the white line to define top of bar
        painter->setPen(Qt::white);
        painter->drawLine(-15, calculateLocalValue(maxValue)-2, 15, calculateLocalValue(maxValue)-2);

        //Draw the white line to define base of bar
        painter->drawLine(-15, calculateLocalValue(minValue)+2, 15, calculateLocalValue(minValue)+2);
    }

	if(!colorStops.isEmpty())
	{
		//If there are color stops, go through them
		foreach(ColorStop colorStop, colorStops)
		{
			//Set pen and brish to color and draw the bar
			painter->setPen(colorStop.color);
			painter->setBrush(colorStop.color);
            if (horizontal) {
                painter->drawRect(QRectF(QPointF(calculateLocalValue(qMin(qMax(colorStop.minValue, minValue), maxValue)),-10), QPointF(calculateLocalValue(qMax(qMin(colorStop.maxValue, maxValue), minValue)),10)));
            } else {
                painter->drawRect(QRectF(QPointF(-10.0, calculateLocalValue(qMin(qMax(colorStop.minValue, minValue), maxValue))), QPointF(10.0, calculateLocalValue(qMax(qMin(colorStop.maxValue, maxValue), minValue)))));
            }


        }
    }



	//Restore the painter with antialising
	painter->restore();

    //Draw Texts around (title, min and max value)
    painter->setPen(Qt::white);
    isPenAlarmColored = false;
    painter->drawText(QRectF(-25, -85, 50, 15), Qt::AlignCenter,titleText);
    painter->drawText(QRectF(-25, -70, 50, 15), Qt::AlignCenter,unitText);
    painter->setPen(Qt::white);
    //painter->drawText(QRectF(-50, -60, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(maxValue, 'f', barPrecision));
    //painter->drawText(QRectF(-50, 40, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(minValue, 'f', barPrecision));

    //  Determine if there should be an alarm
    for (int i=0;i<colorStops.count();i++) {
        if ((currentValue >= colorStops[i].minValue && currentValue <= colorStops[i].maxValue) && (colorStops[i].color == Qt::red || colorStops[i].color == Qt::yellow)) {
            painter->setPen(colorStops[i].color);
            isPenAlarmColored = true;
            if (colorStops[i].color == Qt::red && isAlarmedRed == false) {

                if (isAlarmedYellow) {
                    emit cancelAlarm(titleText);
                    isAlarmedYellow = false;
                }

                emit sendAlarm(titleText, colorStops[i].color, true);
                isAlarmedRed = true;

            } else if (colorStops[i].color == Qt::yellow && isAlarmedYellow == false) {

                if (isAlarmedRed) {
                    emit cancelAlarm(titleText);
                    isAlarmedRed = false;
                }

                emit sendAlarm(titleText, colorStops[i].color, true);
                isAlarmedYellow = true;

            }
        }
    }

    //Set readout details
    font.setBold(true);
    font.setPointSize(18);
    painter->setFont(font);

    if (isAlarmedRed) {
        if (flashState) {
            painter->setPen(Qt::red);
            painter->setBrush(Qt::red);
            painter->drawRect(QRectF(-40, 55, 80, 22));
            painter->setPen(Qt::white);
            painter->drawText(QRectF(-40, 55, 80, 20), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));

         } else {
            painter->setPen(Qt::red);
            painter->drawText(QRectF(-40, 55, 80, 22), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));
        }
    } else if (isAlarmedYellow) {
        if (flashState) {
            painter->setPen(Qt::yellow);
            painter->setBrush(Qt::yellow);
            painter->drawRect(QRectF(-40, 55, 80, 22));
            painter->setPen(Qt::black);
            painter->drawText(QRectF(-40, 55, 80, 22), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));

         } else {
            painter->setPen(Qt::yellow);
            painter->drawText(QRectF(-40, 55, 80, 22), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));
        }
    } else {
        painter->setPen(Qt::white);
        painter->drawText(QRectF(-40, 55, 80, 22), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));
    }

    if (isPenAlarmColored == false) {
        if (isAlarmedRed == true || isAlarmedYellow == true) {
            emit cancelAlarm(titleText);
            isAlarmedRed = false;
            isAlarmedYellow = false;
        }
    }

	//Draw marker
	if((currentValue>minValue) && (currentValue<maxValue))
	{
		painter->setPen(Qt::black);
		painter->setBrush(Qt::white);
		QPolygonF marker;
        if (horizontal) {
            marker.append(QPointF(0.0,-10));
            marker.append(QPointF(-7.0,-20));
            marker.append(QPointF(7.0,-20));
            painter->translate(QPointF(calculateLocalValue(currentValue),0.0));
        } else {
            marker.append(QPointF(-10,0.0));
            marker.append(QPointF(20,-7.0));
            marker.append(QPointF(20,7.0));
            painter->translate(QPointF(0.0,calculateLocalValue(currentValue)));
        }

		painter->drawPolygon(marker);
	}


    update();
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
}

void BarGraph::addColorStop(ColorStop stop)
{
	colorStops.append(stop);
}

void BarGraph::changeFlashState()
{
    if (flashState == false) {
        flashState  = true;
    } else {
        flashState = false;
    }
}
