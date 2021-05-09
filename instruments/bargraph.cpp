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
    smoothData.setSampleSize(20);

    updateTimer.start(100);

    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
}

QRectF BarGraph::boundingRect() const
{
    return QRectF(-45, -75, 85, 160);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void BarGraph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//Save thje painter and deactivate Antialising for rectangle drawing
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, false);
	painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    changeFlashState();

	//Draw ticks with values
//	painter->setPen(Qt::white);
//	foreach(double value, beetweenValues)
//	{
//		painter->drawLine(QPointF(-20.0, calculateLocalValue(value)), QPointF(-10.0, calculateLocalValue(value)));
//		painter->drawText(QRectF(-50, calculateLocalValue(value)-10.0, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(value, 'f', barPrecision));
//	}

    if (minValue == 0.0 || maxValue == 0.0) {
        setBorders(gauge.getMin(), gauge.getMax());
    }

	//Define pen, brush and rect for the bar
    painter->setPen(Qt::green);
    painter->setBrush(Qt::green);
    if (horizontal) {
        if (dataIsValid) {
            painter->drawRect(QRectF(QPointF(-50.0, -10.0), QPointF(50.0, 10.0)));
        } else {
            painter->setPen(Qt::red);
            painter->setBrush(Qt::red);
            painter->drawLine(QPointF(-50.0, -10.0), QPointF(50.0, 10.0));
            painter->drawLine(QPointF(50.0, -10.0), QPointF(-50.0, 10.0));
        }
    } else {
        if (dataIsValid) {
            painter->drawRect(QRectF(QPointF(-10.0, calculateLocalValue(maxValue)), QPointF(10.0, calculateLocalValue(minValue))));

            //Draw the white line to define top of bar
            painter->setPen(Qt::white);
            painter->drawLine(-15, calculateLocalValue(maxValue)-2, 15, calculateLocalValue(maxValue)-2);

            //Draw the white line to define base of bar
            painter->drawLine(-15, calculateLocalValue(minValue)+2, 15, calculateLocalValue(minValue)+2);
        } else {
            painter->setPen(Qt::red);
            painter->setBrush(Qt::red);
            painter->drawLine(QPointF(-10.0, calculateLocalValue(maxValue)), QPointF(10.0, calculateLocalValue(minValue)));
            painter->drawLine(QPointF(10.0, calculateLocalValue(maxValue)), QPointF(-10.0, calculateLocalValue(minValue)));
        }
    }

    //Restore the painter with antialising
    painter->restore();

    isPenAlarmColored = false;

    if (dataIsValid) {
        if (gauge.getName() != "") {
            i=0;
            numOfRanges = gauge.getNRange();

            for (i=0; i<numOfRanges; i++) {
                start = gauge.definitions[i].start;
                end = gauge.definitions[i].end;
                color = gauge.definitions[i].color;

                //Set pen and brush to color and draw the bar
                painter->setPen(color);
                painter->setBrush(color);

                //Save the painter and deactivate Antialising for rectangle drawing
                painter->save();
                painter->setRenderHint(QPainter::Antialiasing, false);
                painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

                if (horizontal) {
                    painter->drawRect(QRectF(QPointF(calculateLocalValue(end),-10), QPointF(calculateLocalValue(start),10)));
                } else {
                    painter->drawRect(QRectF(QPointF(-10.0, calculateLocalValue(end)), QPointF(10.0, calculateLocalValue(start))));
                }

                //Restore the painter with antialising
                painter->restore();

                if (currentValue >= start && currentValue < end) {
                    if (color == Qt::red) {
                        if (isAlarmedRed == false) {
                            emit sendAlarm(titleText, color, true);
                            isAcknowledged = false;
                        }

                        isAlarmedRed = true;
                        isAlarmedYellow = false;
                        isPenAlarmColored = true;

                    } else if (color == Qt::yellow) {
                        if (isAlarmedYellow == false) {
                            emit sendAlarm(titleText, color, true);
                            isAcknowledged = false;
                        }

                        isAlarmedRed = false;
                        isAlarmedYellow = true;
                        isPenAlarmColored = true;

                    }
                }
            }
        }

        if (currentValue < minValue) {
            if (minValue == gauge.definitions[0].start) {
                color = gauge.definitions[0].color;

                if (color == Qt::yellow) {
                    isPenAlarmColored = true;
                    isAlarmedYellow = true;
                    isAlarmedRed = false;
                } else if (color == Qt::red) {
                    isPenAlarmColored = true;
                    isAlarmedRed = true;
                    isAlarmedYellow = false;
                }
            }
        } else if (currentValue > maxValue) {
            if (numOfRanges > 0) {
                if (maxValue == gauge.definitions[numOfRanges-1].end) {
                    color = gauge.definitions[numOfRanges-1].color;

                    if (color == Qt::yellow) {
                        if(isAlarmedYellow == false) {
                            emit sendAlarm(titleText, Qt::yellow, true);
                            isAcknowledged = false;
                        }
                        isPenAlarmColored = true;
                        isAlarmedYellow = true;
                        isAlarmedRed = false;

                    } else if (color == Qt::red) {
                        if(isAlarmedRed == false) {
                            emit sendAlarm(titleText, Qt::red, true);
                            isAcknowledged = false;
                        }
                        isPenAlarmColored = true;
                        isAlarmedRed = true;
                        isAlarmedYellow = false;
                    }
                }
            }

        }
    }

    //Draw Texts around (title, min and max value)
    painter->setPen(Qt::white);
    painter->drawText(QRectF(-30, calculateLocalValue(maxValue)-35, 60, 15), Qt::AlignCenter,titleText);
    painter->drawText(QRectF(-25, calculateLocalValue(maxValue)-20, 50, 15), Qt::AlignCenter,unitText);

    //Set readout details
    font.setBold(true);
    font.setPointSize(18);
    painter->setFont(font);

    if (dataIsValid) {
        if (isAlarmedRed) {
            if (flashState || isAcknowledged) {
                painter->setPen(Qt::red);
                painter->setBrush(Qt::red);
                painter->drawRect(QRectF(-30, 55, 60, 22));
                painter->setPen(Qt::white);
                painter->drawText(QRectF(-30, 55, 60, 20), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));

             } else {
                painter->setPen(Qt::red);
                painter->drawText(QRectF(-30, 55, 60, 22), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));
            }
        } else if (isAlarmedYellow) {
            if (flashState || isAcknowledged) {
                painter->setPen(Qt::yellow);
                painter->setBrush(Qt::yellow);
                painter->drawRect(QRectF(-30, 55, 60, 22));
                painter->setPen(Qt::black);
                painter->drawText(QRectF(-30, 55, 60, 22), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));

             } else {
                painter->setPen(Qt::yellow);
                painter->drawText(QRectF(-30, 55, 60, 22), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));
            }
        } else {
            painter->setPen(Qt::white);
            painter->drawText(QRectF(-30, 55, 60, 22), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));
        }

        if (isPenAlarmColored == false) {
            if (isAlarmedRed == true || isAlarmedYellow == true) {
                emit cancelAlarm(titleText);
                isAlarmedRed = false;
                isAlarmedYellow = false;
                isAcknowledged = false;
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
                if (indicatorSide=="right") {
                    marker.append(QPointF(-10,0.0));
                    marker.append(QPointF(20,-7.0));
                    marker.append(QPointF(20,7.0));
                } else {
                    marker.append(QPointF(10,0.0));
                    marker.append(QPointF(-20,-7.0));
                    marker.append(QPointF(-20,7.0));
                }
                painter->translate(QPointF(0.0,calculateLocalValue(currentValue)));
            }

            painter->drawPolygon(marker);
        }
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

void BarGraph::setBorders(qreal minimum, qreal maximum)
{
	minValue = minimum;
	maxValue = maximum;
}

void BarGraph::setPrecision(quint8 readout, quint8 bar)
{
	readoutPrecision = readout;
	barPrecision = bar;
}

void BarGraph::addBetweenValue(qreal value)
{
	beetweenValues.append(value);
}

double BarGraph::calculateLocalValue(double value) const
{
    return -(value-minValue)/(maxValue-minValue)*75.0+50.0;
}

void BarGraph::setValue(qreal value, qreal rawValue)
{
    checkDataIsValid(value);

    if (smooth) {
        currentValue = smoothData.dsp_ema_double(value);
    } else {
        currentValue = value;
    }

    currentRawValue = rawValue;
}

void BarGraph::addColorStop(ColorStop stop)
{
	colorStops.append(stop);
}

void BarGraph::changeFlashState()
{
    if (QTime::currentTime().second() % 2 == 0) {
        flashState  = true;
    } else {
        flashState = false;
    }
}

void BarGraph::setIndicatorSide(QString side)
{
    indicatorSide = side;
}

void BarGraph::onAlarmAck() {
    if (isPenAlarmColored) {
        isAcknowledged = true;
    }
}

void BarGraph::onUpdate() {
    update();
}

void BarGraph::checkDataIsValid(qreal value) {
    if (value == -999) {
        dataIsValid = false;
    } else {
        dataIsValid = true;
    }
}
