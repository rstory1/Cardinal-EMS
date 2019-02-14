//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Generic circular gauge that can be customized as needed              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef CIRCULARGAUGE_H
#define CIRCULARGAUGE_H

#include <QtWidgets>
#include <QtCore>

//! CircularGauge Class
/*!
 * This class creates generic circular gauge to display information from a single sensor. Currently unused.
*/

class circularGauge : public QGraphicsItem
{
public:
    circularGauge(QGraphicsItem * parent = 0);
    ~circularGauge();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setStartSpan(double start, double span);
	void setBorders(double minimum, double maximum, double greenBorder, double redBorder);
	void addBetweenValue(double value);
	void setValue(double value);
    double getValue() {return currentValue;};
private:
	double calculateLocalValue(double value) const;
	double minValue, maxValue, currentValue;
    double whiteGreenBorder, greenRedBorder, yellowRedBorder, greenYellowBorder, redYellowBorder, yellowGreenBorder;
	double startAngle, spanAngle;
	QList<double> beetweenValues;
    QSettings gaugeSettings;
    void paintWarmup(QPainter parentPainter);
    void paintNormal(QPainter parentPainter);
    bool isWarmup;

};

#endif // CIRCULARGAUGE_H
