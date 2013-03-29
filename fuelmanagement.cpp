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

#include "fuelmanagement.h"

FuelManagement::FuelManagement(QGraphicsItem *parent)
	: QObject()
	, QGraphicsItem(parent)
	, remainingFuelRect(0, 10, 210, 35)
	, remainingFuelAtDestinationRect(0, 45, 210, 35)
	, fuelFlowRect(0, 80, 210, 35)
	, fuelingRect(10, 115, 70, 35)
	, homeRect(120, 115, 70, 35)
	, addLitersTextRect(0, 45, 50, 35)
	, add50LitersRect(50, 45, 40, 35)
	, add10LitersRect(90, 45, 40, 35)
	, add5LitersRect(130, 45, 40, 35)
	, add1LitersRect(170, 45, 40, 35)
	, clearRect(0, 80, 105, 35)
	, fuelTopRect(105, 80, 105, 35)
	, currentMode(fuelModeManagement)
{
}

QRectF FuelManagement::boundingRect() const
{
	return QRectF(0, 0, 210, 160);
}

void FuelManagement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

	//Save thje painter and deactivate Antialising for rectangle drawing
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, false);
	painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

	QPen edgePen(Qt::gray, 2);

	//Draw fuel management
	switch(currentMode)
	{
	case fuelModeManagement:
		painter->setBrush(Qt::black);
		painter->setPen(Qt::transparent);
		painter->drawRect(boundingRect());

		painter->setBrush(Qt::red);
		painter->setPen(edgePen);
		painter->drawRect(remainingFuelRect);
		painter->drawRect(remainingFuelAtDestinationRect);
		painter->drawRect(fuelFlowRect);
		painter->drawRect(fuelingRect);
		painter->drawRect(homeRect);

		painter->setPen(Qt::white);
		painter->drawText(remainingFuelRect, Qt::AlignVCenter | Qt::AlignLeft, "Remaining Fuel:");
		painter->drawText(remainingFuelAtDestinationRect, Qt::AlignVCenter | Qt::AlignLeft, "Remaining Fuel at Destination:");
		painter->drawText(fuelFlowRect, Qt::AlignVCenter | Qt::AlignLeft, "Fuel flow:");

		painter->drawText(remainingFuelRect, Qt::AlignVCenter | Qt::AlignRight, "94 l");
		painter->drawText(remainingFuelAtDestinationRect, Qt::AlignVCenter | Qt::AlignRight, "12 l");
		painter->drawText(fuelFlowRect, Qt::AlignVCenter | Qt::AlignRight, "33.4 l/h");

		painter->drawText(fuelingRect, Qt::AlignCenter, "Fueling");
		painter->drawText(homeRect, Qt::AlignCenter, "Home");
		break;
	case fuelModeFueling:
		painter->setBrush(Qt::black);
		painter->setPen(Qt::transparent);
		painter->drawRect(boundingRect());

		painter->setBrush(Qt::red);
		painter->setPen(edgePen);
		painter->drawRect(remainingFuelRect);
		painter->drawRect(homeRect);

		painter->drawRect(add50LitersRect);
		painter->drawRect(add10LitersRect);
		painter->drawRect(add5LitersRect);
		painter->drawRect(add1LitersRect);

		painter->drawRect(clearRect);
		painter->drawRect(fuelTopRect);

		painter->setPen(Qt::white);
		painter->drawText(remainingFuelRect, Qt::AlignVCenter | Qt::AlignLeft, "Remaining Fuel:");
		painter->drawText(remainingFuelRect, Qt::AlignVCenter | Qt::AlignRight, "94 l");
		painter->drawText(addLitersTextRect, Qt::AlignVCenter| Qt::AlignLeft, "Add Fuel\nin liters");
		painter->drawText(add50LitersRect, Qt::AlignCenter, "+50");
		painter->drawText(add10LitersRect, Qt::AlignCenter, "+10");
		painter->drawText(add5LitersRect, Qt::AlignCenter, "+5");
		painter->drawText(add1LitersRect, Qt::AlignCenter, "+1");

		painter->drawText(clearRect, Qt::AlignCenter, "CLEAR");
		painter->drawText(fuelTopRect, Qt::AlignCenter, "FUEL TOP");

		painter->drawText(homeRect, Qt::AlignCenter, "Home");
		break;
	}

	painter->restore();
}

void FuelManagement::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(homeRect.contains(event->pos()))
	{
		removeOverlay();
		currentMode = fuelModeManagement;
		update();
	}
	if(currentMode == fuelModeManagement)
	{
		if(fuelingRect.contains(event->pos()))
		{
			currentMode = fuelModeFueling;
			update();
		}
	}
	else if(currentMode == fuelModeFueling)
	{
		if(add50LitersRect.contains(event->pos()))
		{
			qDebug() << "+50";
		}
		else if(add10LitersRect.contains(event->pos()))
		{
			qDebug() << "+10";
		}
		else if(add5LitersRect.contains(event->pos()))
		{
			qDebug() << "+5";
		}
		else if(add1LitersRect.contains(event->pos()))
		{
			qDebug() << "+1";
		}
		else if(fuelTopRect.contains(event->pos()))
		{
			qDebug() << "TopUp";
		}
		else if(clearRect.contains(event->pos()))
		{
			qDebug() << "Clear";
		}
	}
}
