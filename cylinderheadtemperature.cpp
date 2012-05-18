#include "cylinderheadtemperature.h"

CylinderHeadTemperature::CylinderHeadTemperature(QGraphicsItem *parent) : QGraphicsItem(parent)
{
}

QRectF CylinderHeadTemperature::boundingRect() const
{
	return QRectF(-125, -125, 250, 250);
}

void CylinderHeadTemperature::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setBrush(Qt::blue);
	painter->drawRect(boundingRect());


}
