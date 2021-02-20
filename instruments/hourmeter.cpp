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

#include "hourmeter.h"

HourMeter::HourMeter(QGraphicsObject *parent) : QGraphicsObject(parent), settings(QCoreApplication::applicationDirPath() + "/ems/settings/settings.ini", QSettings::IniFormat, parent)
{
    hobbs.hour = 0;
    hobbs.min = 0;
    hobbs.sec = 0;

    flight.hour = 0;
    flight.min = 0;
    flight.sec = 0;

    double savedHobbs = settings.value("Time/hobbs", "0.0").toDouble();
    hobbs.hour = floor(savedHobbs);
    hobbs.min = (savedHobbs - floor(savedHobbs)) * 60.0;
    hobbs.sec = (((savedHobbs - floor(savedHobbs)) * 60.0) - hobbs.min) * 60;

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(onShutdown()));

}

QRectF HourMeter::boundingRect() const
{
    return QRectF(-50, -25, 100, 85);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void HourMeter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Save thje painter and deactivate Antialising for rectangle drawing
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    //Define pen, brush and rect for the bar
    painter->setPen(Qt::gray);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect());

    //Restore the painter with antialising
    painter->restore();

    //Draw Texts around (title, min and max value)
    painter->setPen(Qt::white);
    painter->drawText(QRectF(-75, -20, 150, 15), Qt::AlignCenter | Qt::AlignVCenter,QString("Hobbs"));
    painter->drawText(QRectF(-75, 20, 150, 15), Qt::AlignCenter | Qt::AlignVCenter,QString("Flight"));
    //painter->drawText(QRectF(-25, -10, 50, 10), Qt::AlignCenter | Qt::AlignVCenter,unitText);
    painter->setPen(Qt::white);

    //Draw readout
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(QRectF(-75, 0, 150, 15), Qt::AlignCenter | Qt::AlignVCenter, hobbsString);
    painter->drawText(QRectF(-75, 40, 150, 15), Qt::AlignCenter | Qt::AlignVCenter, flightString);
}

void HourMeter::onTic() {


    if (engineState) {
        if (hobbs.sec < 59) {
            hobbs.sec = hobbs.sec + 1;

        } else {
            hobbs.sec = 0;

            if (hobbs.min < 59) {
                hobbs.min = hobbs.min + 1;

                settings.setValue("Time/hobbs", double(hobbs.hour) + (double(hobbs.min)/60));
            } else {
                hobbs.min = 0;
                hobbs.hour = hobbs.hour + 1;

                settings.setValue("Time/hobbs", double(hobbs.hour));
            }


        }

        if (isFlying) {
            if (flight.sec < 59) {
                flight.sec = flight.sec + 1;
            } else {
                flight.sec = 0;

                if (flight.min < 59) {
                    flight.min = flight.min + 1;
                } else {
                    flight.min = 0;
                    flight.hour = flight.hour + 1;
                }
            }
        }
    }

    hobbsString = QString::number(hobbs.hour, 'f', 0).append(QString(".").append(QString::number((hobbs.min/6.0)+(hobbs.sec/360.0), 'f',0)));
    flightString = QString::number(flight.hour, 'f', 0).rightJustified(2,'0').append(QString(":").append(QString::number(flight.min, 'f',0).rightJustified(2,'0'))).append(QString(":").append(QString::number(flight.sec, 'f',0).rightJustified(2,'0')));

    update();
}

void HourMeter::onShutdown() {
    settings.setValue("Time/hobbs", double(hobbs.hour) + (double(hobbs.min)/60) + (double(hobbs.sec)/3600));
}

QString HourMeter::getFlightTime() {
    return flightString;
}

QString HourMeter::getHobbsTime() {
    return hobbsString;
}

void HourMeter::setEngineOn(bool state) {
    engineState = state;
    isFlying = state;
}

void HourMeter::onHobbsINIChanged() {
    double savedHobbs = settings.value("Time/hobbs", "0.0").toDouble();
    hobbs.hour = floor(savedHobbs);
    hobbs.min = (savedHobbs - floor(savedHobbs)) * 60.0;
    hobbs.sec = (((savedHobbs - floor(savedHobbs)) * 60.0) - hobbs.min) * 60;
}
