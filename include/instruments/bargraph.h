//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2012 Tobias Rad                                        //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QtWidgets>
#include <gaugesettings.h>
#include <datasmoothing.h>

//! Bar Graph Class
/*!
 * This class creates a bar graph gauge to be used to display a single sensor reading.
*/

struct ColorStop
{
	ColorStop() : color(QColor()), minValue(0.0), maxValue(0.0) {};
	ColorStop(QColor col, double minVal, double maxVal) : color(col), minValue(minVal), maxValue(maxVal) {};
	QColor color;
	double minValue, maxValue;
};

class BarGraph : public QGraphicsObject
{
	Q_OBJECT
public:
	explicit BarGraph(QGraphicsObject* parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setTitle(QString title);
	void setUnit(QString unit);
	void setBorders(double minimum, double maximum);
	void setPrecision(quint8 readout = 0, quint8 bar = 0);
	void addBetweenValue(double value);
	void addColorStop(ColorStop stop);
    void setValue(qreal value);
    double getValue() {return currentValue;}
    QString gaugeName;
    void setIndicatorSide(QString side);
    void setGaugeType(QString type) {
        gaugeType = type;
        gauge.setGauge(gaugeType);
    }

    void setSmoothBool(bool tf) {
        smooth=tf;
    }

    void checkDataIsValid(qreal value);
    bool dataIsValid = false;

public slots:
	void makeVisible() {setVisible(true);};
    void makeInvisible() {setVisible(false);};
    void changeFlashState();
    void onAlarmAck();
private:
	double calculateLocalValue(double value) const;
	QString titleText, unitText;
    double minValue, maxValue;
    qreal currentValue;
    qreal rawValue;
	QList<double> beetweenValues;
	quint8 barPrecision, readoutPrecision;
	QList<ColorStop> colorStops;
    bool isAlarmedRed = false;
    bool isAlarmedYellow = false;
    bool flashState = false;
    bool isPenAlarmColored = false;
    QPen pen;
    QFont font;
    bool horizontal=false;
    QString indicatorSide = "right";
    bool isAcknowledged = false;
    GaugeSettings gauge;
    QString gaugeType;

    int i;
    int numOfRanges;
    float start;
    float end;
    QColor color;

    DataSmoothing smoothData;
    bool smooth = false;

    QTimer updateTimer;
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *)
	{
		emit hasBeenClicked();
	}
signals:
    void hasBeenClicked();
    void sendAlarm(QString, QColor, bool);
    void cancelAlarm(QString);

private slots:
    void onUpdate();
};

#endif // BARGRAPH_H
