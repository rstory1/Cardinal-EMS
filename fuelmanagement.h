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

#ifndef FUELMANAGEMENT_H
#define FUELMANAGEMENT_H

#include <QtWidgets>

class FuelManagement : public QGraphicsObject
{
	Q_OBJECT
public:
	explicit FuelManagement(QGraphicsObject* parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setFuelFlow(double value);
	void setTimeToDestination(double time);
public slots:
	void activateOverlay()
	{
		setVisible(true);
	}
	void removeOverlay()
	{
		setVisible(false);
	}
	void saveFuelState()
	{
		settings.setValue("Fueling/LastShutdown", fuelAmount);
	}
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
	enum fuelingMode
	{
		fuelModeManagement,
		fuelModeFueling
	};
	QSettings settings;
	double fuelAmount;
	double fuelFlow;
	double timeToDestination;
	fuelingMode currentMode;
	QRectF remainingFuelRect;
	QRectF remainingFuelAtDestinationRect;
	QRectF fuelFlowRect;
	QRectF fuelingRect;
	QRectF homeRect;
	QRectF addLitersTextRect;
	QRectF add50LitersRect;
	QRectF add10LitersRect;
	QRectF add5LitersRect;
	QRectF add1LitersRect;
	QRectF clearRect;
	QRectF fuelTopRect;
};

#endif // FUELMANAGEMENT_H
