#ifndef CYLINDERHEADTEMPERATURE_H
#define CYLINDERHEADTEMPERATURE_H

#include <QtGui>

class CylinderHeadTemperature : public QGraphicsItem
{
public:
	explicit CylinderHeadTemperature(QGraphicsItem * parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // CYLINDERHEADTEMPERATURE_H
