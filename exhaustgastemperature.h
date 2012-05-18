#ifndef EXHAUSTGASTEMPERATURE_H
#define EXHAUSTGASTEMPERATURE_H

#include <QtGui>

class ExhaustGasTemperature : public QGraphicsItem
{
public:
	explicit ExhaustGasTemperature(QGraphicsItem * parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setBorders(double minimum, double maximum, double yellowBorder, double redBorder);
	void addBetweenValue(double value);
	void setValues(double val1, double val2, double val3, double val4);
	void setLeanWindow(double value);
private:
	double calculateLocalValue(double value) const;
	double minValue, maxValue;
	double greenYellowValue, yellowRedValue;
	double leanWindow, leanMinValue;
	bool leanAssistActive;
	QList<double> peakValues;
	QList<bool> peakFound;
	QList<quint8> peakOrder;
	QList<double> currentValues;
	QList<double> betweenValues;
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		leanAssistActive = !leanAssistActive;
		if(leanAssistActive)
		{
			QList<double> egts = currentValues;
			qSort(egts.begin(), egts.end());
			leanMinValue = qMax(egts.first() - 50.0, minValue);
			peakValues = currentValues;
			peakFound.clear();
			peakFound << false << false << false << false;
			peakOrder.clear();
			peakOrder << 0 << 0 << 0 << 0;
		}
	}
};

#endif // EXHAUSTGASTEMPERATURE_H
