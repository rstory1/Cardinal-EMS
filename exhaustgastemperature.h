#ifndef EXHAUSTGASTEMPERATURE_H
#define EXHAUSTGASTEMPERATURE_H

#include <QtGui>

class ExhaustGasTemperature : public QGraphicsItem
{
public:
	explicit ExhaustGasTemperature(QGraphicsItem * parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // EXHAUSTGASTEMPERATURE_H
