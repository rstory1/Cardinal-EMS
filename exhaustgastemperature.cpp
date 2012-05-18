#include "exhaustgastemperature.h"

ExhaustGasTemperature::ExhaustGasTemperature(QGraphicsItem *parent) : QGraphicsItem(parent)
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
	return QRectF(-130, -175, 255, 300);
}

void ExhaustGasTemperature::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setClipRect(boundingRect());
	painter->setPen(QPen(Qt::transparent, 0));

	//Draw the side legend
	painter->setBrush(Qt::green);
	painter->drawRect(QRectF(QPointF(-80.0, calculateLocalValue(minValue)), QPointF(-50.0, calculateLocalValue(greenYellowValue))));
	painter->setBrush(Qt::yellow);
	painter->drawRect(QRectF(QPointF(-80.0, calculateLocalValue(greenYellowValue)), QPointF(-50.0, calculateLocalValue(yellowRedValue))));
	painter->setBrush(Qt::red);
	painter->drawRect(QRectF(QPointF(-80.0, calculateLocalValue(yellowRedValue)), QPointF(-50.0, calculateLocalValue(maxValue))));

	//Set painter for texts
	painter->setPen(QPen(Qt::white, 1));
	painter->setFont(QFont("Arial", 12));

	//Draw the static texts
	painter->drawText(QRectF(-130.0, -162.5, 35.0, 20.0), Qt::AlignLeft | Qt::AlignVCenter, "EGT");
	painter->drawText(QRectF(-130.0, 105.0, 35.0, 20.0), Qt::AlignLeft | Qt::AlignVCenter, "°C");

	//Draw the numbers
	if(leanAssistActive)
	{
		double start = leanMinValue - fmod(leanMinValue, leanWindow/4.0);
		for(double value = start; value < leanMinValue + leanWindow; value += leanWindow/4.0)
		{
			painter->drawLine(-80, calculateLocalValue(value), -90, calculateLocalValue(value));
			painter->drawText(QRectF(-130.0, calculateLocalValue(value)-10.0, 35.0, 20.0), Qt::AlignLeft | Qt::AlignVCenter, QString::number(value, 'f', 0));
		}
	}
	else
	{
		foreach(double value, betweenValues)
		{
			painter->drawLine(-80, calculateLocalValue(value), -90, calculateLocalValue(value));
			painter->drawText(QRectF(-130.0, calculateLocalValue(value)-10.0, 35.0, 20.0), Qt::AlignLeft | Qt::AlignVCenter, QString::number(value, 'f', 0));
		}
	}

	//Draw the red line
	painter->setPen(Qt::red);
	painter->drawLine(-35, calculateLocalValue(yellowRedValue), 125, calculateLocalValue(yellowRedValue));

	//Draw center dashed line
	QPen middleLinePen(Qt::white, 1, Qt::DashLine);
	painter->setPen(middleLinePen);
	for(int i = 0; i < 4; i++)
	{
		painter->drawLine(i*40-15, -125, i*40-15, 125);
	}

	//Draw the bar graphes
	for(int i = 0; i < 4; i++)
	{
		QRectF barRect = QRectF(QPointF(i*40-30, 125), QPointF(i*40-0, calculateLocalValue(currentValues.value(i))));
		if(currentValues.at(i) > yellowRedValue)
		{
			painter->setBrush(Qt::red);
		}
		else if(currentValues.at(i) > greenYellowValue)
		{
			painter->setBrush(Qt::yellow);
		}
		else
		{
			painter->setBrush(Qt::green);
		}
		painter->setPen(QPen(Qt::transparent, 0));
		if(!leanAssistActive && (currentValues.at(i) > minValue) &&
				(currentValues.at(i) < maxValue))
		{
			painter->drawRect(barRect);
		}
		else if(leanAssistActive)
		{
			if(currentValues.at(i) < leanMinValue)
			{
				if((currentValues.at(i) < peakValues.at(0)-leanWindow) ||
						(currentValues.at(i) < peakValues.at(1)-leanWindow) ||
						(currentValues.at(i) < peakValues.at(2)-leanWindow) ||
						(currentValues.at(i) < peakValues.at(3)-leanWindow))
				{
					leanAssistActive = false;
				}
				else
				{
					leanMinValue = currentValues.at(i);
				}
			}
			else if(currentValues.at(i) > (leanMinValue + leanWindow))
			{
				leanMinValue = currentValues.at(i) - leanWindow;
			}
			if(leanAssistActive && peakFound.at(i))
			{
				painter->setBrush(Qt::blue);
			}
			painter->drawRect(barRect);
		}
		else
		{
			painter->setPen(QPen(Qt::red, 2));
			painter->drawLine(barRect.left(), 125, barRect.right(), -125);
			painter->drawLine(barRect.left(), -125, barRect.right(), 125);
		}
		painter->setPen(painter->brush().color());
		if(painter->brush().color() == Qt::green)
		{
			painter->setPen(Qt::white);
		}
		QRectF textRect(-30, -20, 60, 40);
		textRect.moveCenter(QPointF(i*40-15, -140));
		if(i%2)
		{
			textRect.translate(QPointF(0.0, -25.0));
		}
		if(leanAssistActive && peakFound.at(i))
		{
			painter->setPen(Qt::blue);
			painter->drawLine(barRect.left(), calculateLocalValue(peakValues.at(i)), barRect.right(), calculateLocalValue(peakValues.at(i)));
			painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i)-peakValues.at(i), 'f', 0), &textRect);
			painter->setBrush(Qt::transparent);
			painter->drawRect(textRect);

			QRectF peakRect(-30, -20, 60, 40);
			peakRect.moveCenter(QPointF(i*40-15, 115));
			painter->setPen(Qt::white);
			painter->drawText(peakRect, Qt::AlignCenter, QString::number(peakOrder.at(i)));
		}
		else
		{
			painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i), 'f', 0));
		}
	}
}

double ExhaustGasTemperature::calculateLocalValue(double value) const
{
	if(leanAssistActive)
	{
		return -(value-leanMinValue)/(leanWindow)*250.0+125.0;
	}
	else
	{
		return -(value-minValue)/(maxValue-minValue)*250.0+125.0;
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
		leanAssistActive = false;
	}
	if(leanAssistActive)
	{
		peakValues.replace(0, qMax(peakValues.at(0), val1));
		peakValues.replace(1, qMax(peakValues.at(1), val2));
		peakValues.replace(2, qMax(peakValues.at(2), val3));
		peakValues.replace(3, qMax(peakValues.at(3), val4));
		for(int i = 0; i < 4; i++)
		{
			if(!peakFound.at(i) &&
					(peakValues.at(i)-5.0 > currentValues.at(i)))
			{
				peakFound.replace(i, true);
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
