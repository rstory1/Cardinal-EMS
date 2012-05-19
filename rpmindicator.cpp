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
	return QRectF(-200.0, -150.0, 400.0, 300.0);
}

void RpmIndicator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//Draw the arc
	QRectF circle = QRectF(-150.0, -150.0, 300.0, 300.0);
	int minAngle = -210.0;
	int whiteGreenAngle = calculateLocalValue(whiteGreenBorder);
	int greenRedAngle = calculateLocalValue(greenRedBorder);
	int maxAngle = 10.0;
	QPen pen(Qt::transparent, 0);
	painter->setPen(pen);
	painter->setBrush(Qt::green);
	painter->drawPie(circle, 210*16.0, -220.0*16.0);
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
	return ((value-maxValue)/(maxValue-minValue)*220.0)+10.0;
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
