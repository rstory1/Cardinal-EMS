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

#include "buttonbar.h"

ButtonBar::ButtonBar(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    gradient1.setColorAt(1, Qt::black);
    gradient1.setColorAt(0, Qt::darkGray);

    gradient2.setColorAt(1, Qt::black);
    gradient2.setColorAt(0, Qt::darkGray);

    gradient3.setColorAt(1, Qt::black);
    gradient3.setColorAt(0, Qt::darkGray);

    gradient4.setColorAt(1, Qt::black);
    gradient4.setColorAt(0, Qt::darkGray);

    gradient5.setColorAt(1, Qt::black);
    gradient5.setColorAt(0, Qt::darkGray);
}

QRectF ButtonBar::boundingRect() const
{
    return QRectF(0.0, -50.0, 600.0, 50.0);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void ButtonBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //painter->setBrush(Qt::darkBlue);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 14, QFont::Bold));

    //Save thje painter and deactivate Antialising for rectangle drawing
    //painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    //painter->restore();

    painter->fillRect(buttonRect1, gradient1);
    painter->fillRect(buttonRect2, gradient2);

    switch(buttonDisplay) {
    case 1: painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);

            painter->drawText(buttonRect1, Qt::AlignCenter,"Fuel");
            painter->drawText(buttonRect2, Qt::AlignCenter,"Settings");

            if (isAlarmFlashing) {
                painter->fillRect(buttonRect3, gradient3);
                painter->drawRect(buttonRect3);
                painter->drawText(buttonRect3, Qt::AlignCenter,"Ack");
            }

            break;

    case 2: painter->fillRect(buttonRect3, gradient3);

            painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);

            painter->drawText(buttonRect1, Qt::AlignCenter,"<-Menu");
            painter->drawText(buttonRect2, Qt::AlignCenter,"+");
            painter->drawText(buttonRect3, Qt::AlignCenter,"-");

            if (isAlarmFlashing) {
                painter->fillRect(buttonRect4, gradient4);
                painter->drawRect(buttonRect4);
                painter->drawText(buttonRect4, Qt::AlignCenter,"Ack");
            }

            break;

    case 3: painter->fillRect(buttonRect3, gradient3);
            painter->fillRect(buttonRect4, gradient4);

            painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);
            painter->drawRect(buttonRect4);

            painter->drawText(buttonRect1, Qt::AlignCenter,"<-Menu");
            painter->drawText(buttonRect2, Qt::AlignCenter,"Export");
            painter->drawText(buttonRect3, Qt::AlignCenter,"Update");
            painter->drawText(buttonRect4, Qt::AlignCenter,"Quit App");

            if (isAlarmFlashing) {
                painter->fillRect(buttonRect5, gradient5);
                painter->drawRect(buttonRect5);
                painter->drawText(buttonRect5, Qt::AlignCenter,"Ack");
            }

            break;
    }

    update();
}

void ButtonBar::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickedPos = event->buttonDownPos(Qt::LeftButton);
    qDebug() << buttonDisplay;
    switch(buttonDisplay) {
    case 1: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            buttonDisplay = 2;

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            buttonDisplay = 3;

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            ackPressed();
        }

        break;

    case 2: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            buttonDisplay = 1;

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            // emit a signal to increase the fuel quantity
            emit sendFuelChange("+");

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            // emit a signal to decrease the fuel quantity
            emit sendFuelChange("-");

        } else if ((clickedPos.x() > buttonRect4.x() && clickedPos.x() < buttonRect4.x() + buttonRect4.width()) && (clickedPos.y() > buttonRect4.y())) {
            ackPressed();
        }

        break;

    case 3: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            buttonDisplay = 1;

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            // emit a signal to show log file export dialog

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            // emit a signal to show a software update dialog
            QFile::copy("app.exe", "update.exe");
        } else if ((clickedPos.x() > buttonRect4.x() && clickedPos.x() < buttonRect4.x() + buttonRect4.width()) && (clickedPos.y() > buttonRect4.y())) {
            quitPressed();

        } else if ((clickedPos.x() > buttonRect5.x() && clickedPos.x() < buttonRect5.x() + buttonRect5.width()) && (clickedPos.y() > buttonRect5.y())) {
            ackPressed();
        }

        break;
    }

    QGraphicsItem::mousePressEvent(event);
}

void ButtonBar::ackPressed() {
    emit sendAlarmAck();
    isAlarmFlashing = false;
}

void ButtonBar::onAlarmFlash() {
    isAlarmFlashing = true;
}
