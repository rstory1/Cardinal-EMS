#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QtGui>

struct ColorStop
{
	ColorStop() : color(QColor()), minValue(0.0), maxValue(0.0) {};
	ColorStop(QColor col, double minVal, double maxVal) : color(col), minValue(minVal), maxValue(maxVal) {};
	QColor color;
	double minValue, maxValue;
};

class BarGraph : public QGraphicsItem
{
public:
	explicit BarGraph(QGraphicsItem * parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setTitle(QString title);
	void setUnit(QString unit);
	void setBorders(double minimum, double maximum);
	void setPrecision(quint8 readout = 0, quint8 bar = 0);
	void addBetweenValue(double value);
	void addColorStop(ColorStop stop);
	void setValue(double value);
private:
	double calculateLocalValue(double value);
	QString titleText, unitText;
	double minValue, maxValue, currentValue;
	QList<double> beetweenValues;
	quint8 barPrecision, readoutPrecision;
	QList<ColorStop> colorStops;
};

#endif // BARGRAPH_H
