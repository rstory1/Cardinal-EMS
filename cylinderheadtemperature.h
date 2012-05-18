#ifndef CYLINDERHEADTEMPERATURE_H
#define CYLINDERHEADTEMPERATURE_H

#include <QtGui>

class CylinderHeadTemperature : public QGraphicsItem
{
public:
	explicit CylinderHeadTemperature(QGraphicsItem * parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setBorders(double minimum, double maximum, double yellowBorder, double redBorder);
	void addBetweenValue(double value);
	void setValues(double val1, double val2, double val3, double val4);
private:
	double calculateLocalValue(double value) const;
	double minValue, maxValue;
	double greenYellowValue, yellowRedValue;
	QList<double> currentValues;
	QList<double> betweenValues;
};

#endif // CYLINDERHEADTEMPERATURE_H
