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

#include "exhaustgastemperature.h"

ExhaustGasTemperature::ExhaustGasTemperature(QGraphicsObject *parent) : QGraphicsObject(parent)
  , minValue(0.0)
  , maxValue(0.0)
  , greenYellowValue(0.0)
  , yellowRedValue(0.0)
  , leanWindow(0.0)
  , leanAssistActive(false)
{
	currentValues << 0.0 << 0.0 << 0.0 << 0.0;
}

QRectF ExhaustGasTemperature::boundingRect() const
{
	return QRectF(-130, -170, 255, 230);
}

void ExhaustGasTemperature::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	//Set clipping rectangle to avoid overdrawing
	painter->setClipRect(boundingRect());
	painter->setPen(QPen(Qt::transparent, 0));

	//Draw the side legend
	painter->setBrush(Qt::green);
	painter->setPen(QPen(Qt::green, 0));
	painter->drawRect(QRectF(QPointF(-80.0, calculateLocalValue(minValue)), QPointF(-50.0, calculateLocalValue(greenYellowValue))));
	painter->setBrush(Qt::yellow);
	painter->setPen(QPen(Qt::yellow, 0));
	painter->drawRect(QRectF(QPointF(-80.0, calculateLocalValue(greenYellowValue)), QPointF(-50.0, calculateLocalValue(yellowRedValue))));
	painter->setBrush(Qt::red);
	painter->setPen(QPen(Qt::red, 0));
	painter->drawRect(QRectF(QPointF(-80.0, calculateLocalValue(yellowRedValue)), QPointF(-50.0, calculateLocalValue(maxValue))));

	//Set painter for texts
	painter->setPen(QPen(Qt::white, 1));
	painter->setFont(QFont("Arial", 12));

	//Draw the static texts
	painter->drawText(QRectF(-130.0, -165.0, 35.0, 20.0), Qt::AlignLeft | Qt::AlignVCenter, "EGT");
	painter->drawText(QRectF(-130.0, 40.0, 35.0, 20.0), Qt::AlignLeft | Qt::AlignBottom, QString::fromUtf8("°C"));

	//Draw the ticks and numbers at the legend
	if(leanAssistActive)
	{
		//If lean assist is active, draw ticks in quarters of the lean window, tick start is below the graph
		double start = leanMinValue - fmod(leanMinValue, leanWindow/4.0);
		for(double value = start; value < leanMinValue + leanWindow; value += leanWindow/4.0)
		{
			painter->drawLine(-80, calculateLocalValue(value), -90, calculateLocalValue(value));
			painter->drawText(QRectF(-130.0, calculateLocalValue(value)-10.0, 35.0, 20.0), Qt::AlignLeft | Qt::AlignVCenter, QString::number(value, 'f', 0));
		}
	}
	else
	{
		//In normal mode, draw values at the defined between values
		foreach(double value, betweenValues)
		{
			painter->drawLine(-80, calculateLocalValue(value), -90, calculateLocalValue(value));
			painter->drawText(QRectF(-130.0, calculateLocalValue(value)-10.0, 35.0, 20.0), Qt::AlignLeft | Qt::AlignVCenter, QString::number(value, 'f', 0));
		}
	}

	//Draw the red line to define warning area
	painter->setPen(Qt::red);
	painter->drawLine(-35, calculateLocalValue(yellowRedValue), 125, calculateLocalValue(yellowRedValue));

	//Draw center dashed line where the graphs are running
	painter->setPen(QPen(Qt::white, 1, Qt::DashLine));
	for(int i = 0; i < 4; i++)
	{
		painter->drawLine(i*40-15, -120, i*40-15, 60);
	}

	//Draw the bar graphs
	for(int i = 0; i < 4; i++)
	{
		QRectF barRect = QRectF(QPointF(i*40-30, 60), QPointF(i*40-0, calculateLocalValue(currentValues.value(i))));
		if(currentValues.at(i) > yellowRedValue)
		{
			//If value is in warning area, bar is drawn red
			painter->setBrush(Qt::red);

            if (isAlarmed == false) {
                //emit sendAlarm("EGT", Qt::red, false);
                isAlarmed = true;
            }
		}
		else if(currentValues.at(i) > greenYellowValue)
		{
			//If value is in caution area, bar is drawn yellow
			painter->setBrush(Qt::yellow);

//            if (isAlarmed == false) {
//                emit sendAlarm("EGT", Qt::yellow, false);
//                isAlarmed = true;
//            }
		}
		else
		{
			//In all other cases, bar is drawn green
			painter->setBrush(Qt::green);

            if (isAlarmed) {
                emit cancelAlarm("EGT");
                isAlarmed = false;
            }

		}
		if(leanAssistActive)
		{
			//If lean assist is active, check what display should occur
			if(currentValues.at(i) < leanMinValue)
			{
				//If current value is below minimum value, check if window is adjustable
				if((currentValues.at(i) < peakValues.at(0)-leanWindow) ||
						(currentValues.at(i) < peakValues.at(1)-leanWindow) ||
						(currentValues.at(i) < peakValues.at(2)-leanWindow) ||
						(currentValues.at(i) < peakValues.at(3)-leanWindow))
				{
					//If value drops below maximum displayable window, disable lean assistant
					leanAssistActive = false;
				}
				else
				{
					//If window is adjustable to keep all peaks in display, adjust the minimum
					leanMinValue = currentValues.at(i);
				}
			}
			else if(currentValues.at(i) > (leanMinValue + leanWindow))
			{
				//If value is above the maximum value adjust the window to display new peak
				leanMinValue = currentValues.at(i) - leanWindow;
			}
			if(leanAssistActive && peakFound.at(i))
			{
				//If peak has already been found, adjust the color to blue
				painter->setBrush(Qt::blue);
			}
		}
		if((currentValues.at(i) > minValue) &&
				(currentValues.at(i) < maxValue))
		{
			//If value is in visible range, draw the bar
			painter->setPen(painter->brush().color());
			painter->drawRect(barRect);
		}
		else
		{
			//If value is outside the displayed range, draw a red cross
			painter->setPen(QPen(Qt::red, 2));
			painter->drawLine(barRect.left(), 60, barRect.right(), -120);
			painter->drawLine(barRect.left(), -120, barRect.right(), 60);
		}
		//Set the pen to the brush color
		painter->setPen(painter->brush().color());
		if(painter->brush().color() == Qt::green)
		{
			//If brush was green, pen should be white
			painter->setPen(Qt::white);
		}
		//Define text position and move to correct column
		QRectF textRect(-30, -20, 60, 40);
		textRect.moveCenter(QPointF(i*40-15, -135));
		if(i%2)
		{
			textRect.translate(QPointF(0.0, -20.0));
		}
		//Draw the readout
		if(leanAssistActive && peakFound.at(i))
		{
			//If peak was found in lean assist draw line at peak temperature
			painter->drawLine(barRect.left(), calculateLocalValue(peakValues.at(i)), barRect.right(), calculateLocalValue(peakValues.at(i)));
			//Draw the difference to the peak temperature
			painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i)-peakValues.at(i), 'f', 0), &textRect);
			//Draw a box around the text with transparent brush
			painter->setBrush(Qt::transparent);
			painter->drawRect(textRect);

			//Define rect and move it to correct column to write the peak order
			QRectF peakRect(-30, -20, 60, 40);
			peakRect.moveCenter(QPointF(i*40-15, 40));
			painter->setPen(Qt::white);
			painter->drawText(peakRect, Qt::AlignHCenter | Qt::AlignBottom, QString::number(peakOrder.at(i)));
		}
		else
		{
			//If in normal mode, just draw the readout
			painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i), 'f', 0));
		}
	}
	if(leanAssistActive)
	{
		painter->setBrush(Qt::transparent);
		painter->setPen(QPen(Qt::red, 6.0));
		painter->drawRect(boundingRect());
	}
}

double ExhaustGasTemperature::calculateLocalValue(double value) const
{
	if(leanAssistActive)
	{
		//If lean assist is active, calculate based on reduced window
		return -(value-leanMinValue)/(leanWindow)*180.0+60.0;
	}
	else
	{
		//Otherwise the full range is used for calculation
		return -(value-minValue)/(maxValue-minValue)*180.0+60.0;
	}
}

void ExhaustGasTemperature::addBetweenValue(double value)
{
	betweenValues.append(value);
}

void ExhaustGasTemperature::setValues(double val1, double val2, double val3, double val4)
{
	currentValues.replace(0, val1);
	currentValues.replace(1, val2);
	currentValues.replace(2, val3);
	currentValues.replace(3, val4);
	if(val1 > greenYellowValue ||
			val2 > greenYellowValue ||
			val3 > greenYellowValue ||
			val4 > greenYellowValue)
	{
		//If any value exceeds caution range, deactivate lean assist
		leanAssistActive = false;
	}
	else if(val1 < minValue ||
			val2 < minValue ||
			val3 < minValue ||
			val4 < minValue)
	{
		//If any value falls below min value, deactivate lean assist
		leanAssistActive = false;
	}
	if(leanAssistActive)
	{
		//If lean assist is active, check if peak values are exceeded
		peakValues.replace(0, qMax(peakValues.at(0), val1));
		peakValues.replace(1, qMax(peakValues.at(1), val2));
		peakValues.replace(2, qMax(peakValues.at(2), val3));
		peakValues.replace(3, qMax(peakValues.at(3), val4));
		for(int i = 0; i < 4; i++)
		{
			if(!peakFound.at(i) &&
					(peakValues.at(i) - 5.0 > currentValues.at(i)))
			{
				//If current temperature is 5K lower than peak value, define peak as found
				peakFound.replace(i, true);
				//Set the count of found peak
				peakOrder.replace(i, peakFound.count(true));
			}
		}
	}
	update();
}

void ExhaustGasTemperature::setBorders(double minimum, double maximum, double yellowBorder, double redBorder)
{
	minValue = minimum;
	maxValue = maximum;
	greenYellowValue = yellowBorder;
	yellowRedValue = redBorder;
}

void ExhaustGasTemperature::setLeanWindow(double value)
{
	leanWindow = value;
}

bool ExhaustGasTemperature::isLeanAssistActive() const
{
	return leanAssistActive;
}
