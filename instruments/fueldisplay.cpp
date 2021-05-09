//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CardinalEMS,   a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2017 Ryan Story                                        //
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

#include "fueldisplay.h"

FuelDisplay::FuelDisplay(QGraphicsObject *parent)
    : QGraphicsObject(parent)
    , settings(QCoreApplication::applicationDirPath() + "/ems/settings/settings.ini", QSettings::IniFormat)
    , fuelAmount(0.0)
    , fuelFlow(-1.0)
    , timeToDestination(-1.0)
    , remainingFuelRect(-95, -75, 90, 55)
    , remainingFuelAtDestinationRect(-95, -80, 90, 55)
    , mpgRect(0, -80, 90, 55)
    , rangeRect(0, -75, 90, 55)
{
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(saveFuelState()));
    fuelAmount = settings.value("Fueling/LastShutdown", 0.0).toDouble();
    fuelUnits = settings.value("Units/fuel", "gal").toString();

    connect(&fuelBurnTimer, SIGNAL(timeout()), this, SLOT(updateFuelBurn()));
    fuelBurnTimer.start(fuelBurnUpdateInterval);
}

QRectF FuelDisplay::boundingRect() const
{
    return QRectF(-100, -100, 200, 85);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void FuelDisplay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    double fuelAtDestination = fuelAmount - (fuelFlow * timeToDestination);

    // This is purely for testing.
    double airspeed = 100;

    //Save the painter and deactivate Antialising for rectangle drawing
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    //Draw fuel display
    painter->setBrush(Qt::black);
    painter->setPen(Qt::gray);
    painter->drawRect(boundingRect());

    if (gpsAvailable) {

        painter->drawRoundedRect(remainingFuelAtDestinationRect, 5, 5);
        painter->drawRoundedRect(mpgRect, 5, 5);

        painter->setPen(Qt::gray);
        painter->drawText(QRectF(mpgRect.left(), mpgRect.top() + 5, mpgRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "NMPG");
        painter->drawText(QRectF(remainingFuelAtDestinationRect.left(), remainingFuelAtDestinationRect.top() + 5, remainingFuelAtDestinationRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "Fuel at\nDestination");

        painter->setFont(QFont("Arial", 18, QFont::Bold));
        painter->setPen(Qt::white);
        painter->drawText(QRectF(remainingFuelAtDestinationRect.left(), remainingFuelAtDestinationRect.top() + 35, remainingFuelAtDestinationRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, QString::number(fuelAtDestination, 'f', 1));
        painter->drawText(QRectF(mpgRect.left(), mpgRect.top() + 35, mpgRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, QString::number(airspeed/fuelFlow, 'f', 1));
    }

    painter->drawRoundedRect(remainingFuelRect, 5, 5);
    painter->drawRoundedRect(rangeRect, 5, 5);

    painter->restore();

    // Draw headings for values
    painter->setPen(Qt::gray);
    painter->drawText(QRectF(-100,-95,200,17.5), Qt::AlignVCenter | Qt::AlignCenter, "FUEL (gal)");
    painter->drawText(QRectF(remainingFuelRect.left(), remainingFuelRect.top() + 2.5, remainingFuelRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "Qty");
    painter->drawText(QRectF(rangeRect.left(), rangeRect.top() + 2.5, rangeRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "Range (hr)");

    // Draw values
    painter->setFont(QFont("Arial", 18, QFont::Bold));
    painter->setPen(Qt::white);
    painter->drawText(QRectF(remainingFuelRect.left(), remainingFuelRect.top() + 35, remainingFuelRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, QString::number(fuelAmount, 'f', 1));

    if (fuelFlow <1.5) {
        painter->drawText(QRectF(rangeRect.left(), rangeRect.top() + 35, rangeRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, "--.-");
    } else {
        painter->drawText(QRectF(rangeRect.left(), rangeRect.top() + 35, rangeRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, QString::number(fuelAmount/fuelFlow,'f',1));
    }

}

void FuelDisplay::setFuelFlow(double value)
{
    if (value == -999) {
        dataIsValid = false;
    } else {
        dataIsValid = true;
    }

    fuelFlow = value;

    update();
}

void FuelDisplay::setTimeToDestination(double time)
{
    timeToDestination = time;
}

void FuelDisplay::onFuelAmountChange()
{
    fuelAmount = settings.value("Fueling/LastShutdown", 0.0).toDouble();
}

void FuelDisplay::updateFuelBurn()
{
    if (fuelFlow != -999) {
        fuelAmount = fuelAmount - (fuelFlow * (fuelBurnUpdateInterval * 0.000000277778));
        emit saveFuelState(fuelAmount);
    }

}
