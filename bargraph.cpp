#include "bargraph.h"

BarGraph::BarGraph(QGraphicsItem *parent) : QGraphicsItem(parent)
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

	//Draw bar
	if(colorStops.isEmpty())
	{
		painter->setBrush(Qt::green);
		painter->drawRect(-10, -50, 20, 100);
	}
	else
	{
		painter->setBrush(Qt::green);
		painter->drawRect(-10, -50, 20, 100);
		foreach(ColorStop colorStop, colorStops)
		{
			QRectF rect(QPointF(-10, calculateLocalValue(qMin(qMax(colorStop.minValue, minValue), maxValue))), QPointF(10, calculateLocalValue(qMax(qMin(colorStop.maxValue, maxValue), minValue))));
			painter->setBrush(colorStop.color);
			painter->drawRect(rect);
		}
	}

	//Draw Texts around
	painter->setPen(Qt::white);
	painter->drawText(QRectF(-20, -70, 40, 20), Qt::AlignCenter, titleText);
	painter->drawText(QRectF(-50, -60, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(maxValue, 'f', barPrecision));
	painter->drawText(QRectF(-50, 40, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(minValue, 'f', barPrecision));

	//Draw lines with values
	foreach(double value, beetweenValues)
	{
		painter->drawLine(QPointF(-20.0, calculateLocalValue(value)), QPointF(-10.0, calculateLocalValue(value)));
		painter->drawText(QRectF(-50, calculateLocalValue(value)-10.0, 30, 20), Qt::AlignVCenter | Qt::AlignRight, QString::number(value, 'f', barPrecision));
	}

	//Draw Readout
	painter->drawText(QRectF(-20, 50, 40, 20), Qt::AlignCenter, QString::number(currentValue, 'f', readoutPrecision));
	painter->drawText(QRectF(10, 50, 20, 20), Qt::AlignCenter, unitText);

	//Draw Marker
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
