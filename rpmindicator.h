#ifndef RPMINDICATOR_H
#define RPMINDICATOR_H

#include <QtGui>

class RpmIndicator : public QGraphicsItem
{
public:
	RpmIndicator(QGraphicsItem * parent = 0);
	~RpmIndicator();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setBorders(double minimum, double maximum, double greenBorder, double redBorder);
	void addBetweenValue(double value);
	void setValue(double value);
private:
	double calculateLocalValue(double value) const;
	double minValue, maxValue, currentValue;
	double whiteGreenBorder, greenRedBorder;
	QList<double> beetweenValues;
};

#endif // RPMINDICATOR_H
