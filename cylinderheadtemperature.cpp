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
	return QRectF(-125, -175, 250, 300);
}

void CylinderHeadTemperature::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(QPen(Qt::transparent, 0));

	//Draw the side legend
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

	//Draw the numbers
	foreach(double value, betweenValues)
	{
		painter->drawLine(80, calculateLocalValue(value), 90, calculateLocalValue(value));
		painter->drawText(QRectF(90.0, calculateLocalValue(value)-10.0, 35.0, 20.0), Qt::AlignRight | Qt::AlignVCenter, QString::number(value, 'f', 0));
	}

	//Draw the red line
	painter->setPen(Qt::red);
	painter->drawLine(-125, calculateLocalValue(yellowRedValue), 35, calculateLocalValue(yellowRedValue));

	//Draw center dashed line
	QPen middleLinePen(Qt::white, 1, Qt::DashLine);
	painter->setPen(middleLinePen);
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
		if((currentValues.at(i) > minValue) &&
				(currentValues.at(i) < maxValue))
		{
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
		textRect.moveCenter(QPointF(i*40-105, -140));
		if(i%2)
		{
			textRect.translate(QPointF(0.0, -25.0));
		}
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
