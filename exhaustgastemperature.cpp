#include "exhaustgastemperature.h"

ExhaustGasTemperature::ExhaustGasTemperature(QGraphicsItem *parent) : QGraphicsItem(parent)
{
}

QRectF ExhaustGasTemperature::boundingRect() const
{
	return QRectF(-125, -175, 250, 300);
}

void ExhaustGasTemperature::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setBrush(Qt::blue);
	painter->drawRect(boundingRect());
}
