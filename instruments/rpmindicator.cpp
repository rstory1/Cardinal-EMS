//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2012 Tobias Rad                                        //
//                2017 Ryan Story                                       //
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

RpmIndicator::RpmIndicator(QGraphicsObject *parent) : QGraphicsObject(parent)
  , minValue(0.0)
  , maxValue(0.0)
  , currentValue(0.0)
  , whiteGreenBorder(0.0)
  , greenRedBorder(0.0)
  , startAngle(0.0)
  , spanAngle(0.0)
{
    isWarmup=true;

    gauge.setGauge("RPM");

    updateTimer.start(100);

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
}


QRectF RpmIndicator::boundingRect() const
{
    return QRectF(-180.0, -160.0, 360.0, 320.0);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void RpmIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
    Q_UNUSED(widget);

	//Draw the arc
    QRectF circle = QRectF(-150.0, -150.0, 300.0, 300.0);

    //Draw the green basis
    painter->setPen(QPen(Qt::green, 0));
    painter->setBrush(Qt::green);
    painter->drawPie(circle, startAngle*16.0, -spanAngle*16.0);

    changeFlashState();

    if (isWarmup) {
        i=0;
        numOfRanges = gauge.warmupNRange;

        for (i=0; i<numOfRanges; i++) {
            startRange = calculateLocalValue(gauge.warmupDefinitions[i].start);
            endRange = calculateLocalValue(gauge.warmupDefinitions[i].end);
            color = gauge.warmupDefinitions[i].color;

            painter->setPen(QPen(color, 0));
            painter->setBrush(color);
            painter->drawPie(circle, startRange * 16, -fabs( endRange - startRange)*16);
        }
    } else {
        i=0;
        numOfRanges = gauge.getNRange();

        for (i=0; i<numOfRanges; i++) {
            startRange = calculateLocalValue(gauge.definitions[i].start);
            endRange = calculateLocalValue(gauge.definitions[i].end);
            color = gauge.definitions[i].color;

            painter->setPen(QPen(color, 0));
            painter->setBrush(color);
            painter->drawPie(circle, startRange * 16.0, -fabs( endRange - startRange)*16.0);
        }
    }

	//Overlay the center with a black circle
	painter->setPen(QPen(Qt::black, 0));
	painter->setBrush(Qt::black);
    painter->drawEllipse(circle.center(), 130.0, 130.0);

	//Set the pen and font to draw the ticks
	painter->setPen(QPen(Qt::white, 2));
    painter->setFont(QFont("Arial", 16));
	foreach(double value, beetweenValues)
	{
		//Rotate painter and draw the ticks
		painter->save();
		painter->rotate(-calculateLocalValue(value));
        painter->drawLine(120, 0, 150, 0);
		painter->restore();
		//Define a box, move it and draw the text centered to this position
        QRectF textRect(-12, -12, 24, 24);
        textRect.moveCenter(QPointF(cos(calculateLocalValue(value)/180.0*M_PI)*100.0, -sin(calculateLocalValue(value)/180.0*M_PI)*100.0));
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
        marker.append(QPointF(130.0, 0.0));
        marker.append(QPointF(160.0, -10.0));
        marker.append(QPointF(160.0, 10.0));

		//Rotate the painter and draw the needle
		painter->save();
		painter->rotate(-calculateLocalValue(currentValue));
		painter->drawPolygon(marker);
		painter->restore();
	}

    i=0;
    painter->setPen(Qt::blue);

    for (i=0; i<numOfRanges; i++) {
        if (isWarmup) {
            startVal = gauge.warmupDefinitions[i].start;
            endVal = gauge.warmupDefinitions[i].end;
            color = gauge.warmupDefinitions[i].color;

        } else {
            startVal = gauge.definitions[i].start;
            endVal = gauge.definitions[i].end;
            color = gauge.definitions[i].color;

        }

        if (startVal <= currentValue && currentValue < endVal) {
            if (color == Qt::red) {
                if (flashState == false && isAcknowledged == false) {

                    painter->setPen(Qt::red);

                } else if (flashState == true || isAcknowledged == true) {
                    painter->setPen(Qt::red);
                    painter->setBrush(Qt::red);

                    painter->drawRect(QRectF(-40, 45, 190, 45));

                    painter->setPen(Qt::white);
                }

                if (isAlarmedRed == false) {
                    //emit sendAlarm("RPM", Qt::red, true);
                    isAlarmedRed = true;
                    isAcknowledged = false;
                }

            } else if (color == Qt::yellow) {
                if (flashState == false && isAcknowledged == false) {

                    painter->setPen(Qt::yellow);

                } else if (flashState == true || isAcknowledged == true) {
                    painter->setPen(Qt::yellow);
                    painter->setBrush(Qt::yellow);

                    painter->drawRect(QRectF(-40, 45, 190, 45));

                    painter->setPen(Qt::black);
                }

                if (isAlarmedYellow == false) {
                    //emit sendAlarm("RPM", Qt::yellow, true);
                    isAlarmedYellow = true;
                    isAcknowledged = false;
                }
            }
        } else if (currentValue > maxValue || currentValue < minValue) {
            if (flashState == false && isAcknowledged == false) {

                painter->setPen(Qt::red);

            } else if (flashState == true || isAcknowledged == true) {
                painter->setPen(Qt::red);
                painter->setBrush(Qt::red);

                painter->drawRect(QRectF(-40, 45, 190, 45));

                painter->setPen(Qt::white);
            }

            if (isAlarmedRed == false) {
                //emit sendAlarm("RPM", Qt::red, true);
                isAlarmedRed = true;
                isAcknowledged = false;
            }
        }
    }

    if (painter->pen().color() != Qt::red && painter->pen().color() != Qt::yellow
            && painter->pen().color() != Qt::white && painter->pen().color() != Qt::black) {
        painter->setPen(Qt::white);

        if (isAlarmedRed == true || isAlarmedYellow == true) {
            //emit cancelAlarm("RPM");
            isAlarmedRed = false;
            isAlarmedYellow = false;
        }
    }

	//Round value to the nearest 10
	QString rpm = QString::number(currentValue-fmod(currentValue, 10.0), 'f', 0);

	//Set position and font for the value and draw it
    QRectF textRect(-100, 35, 170, 65);
    painter->setFont(QFont("Arial", 30, QFont::Bold));
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

void RpmIndicator::setBorders(double minimum, double maximum)
{
    minValue = minimum;
    maxValue = maximum;
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
    if (value == -999) {
        dataIsValid = false;
    } else {
        dataIsValid = true;
    }

	currentValue = value;

}

void RpmIndicator::changeFlashState()
{
    if (QTime::currentTime().second() % 2 == 0) {
        flashState  = true;
    } else {
        flashState = false;
    }

}
