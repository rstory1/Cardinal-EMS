#include "buttonbar.h"

ButtonBar::ButtonBar(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    gradient1.setColorAt(1, Qt::black);
    gradient1.setColorAt(0, Qt::darkGray);

    gradient2.setColorAt(1, Qt::black);
    gradient2.setColorAt(0, Qt::darkGray);

    gradient3.setColorAt(1, Qt::black);
    gradient3.setColorAt(0, Qt::darkGray);
}

QRectF ButtonBar::boundingRect() const
{
    return QRectF(0.0, -50.0, 600.0, 50.0);
}


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
    painter->fillRect(buttonRect3, gradient3);

    switch(buttonDisplay) {
    case 1: painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);

            painter->drawText(buttonRect1, Qt::AlignCenter,"Ack");
            painter->drawText(buttonRect2, Qt::AlignCenter,"Fuel");
            painter->drawText(buttonRect3, Qt::AlignCenter,"Settings");
            break;

    case 2: painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);

            painter->drawText(buttonRect1, Qt::AlignCenter,"<-Menu");
            painter->drawText(buttonRect2, Qt::AlignCenter,"+");
            painter->drawText(buttonRect3, Qt::AlignCenter,"-");
            break;

    case 3: painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);

            painter->drawText(buttonRect1, Qt::AlignCenter,"<-Menu");
            painter->drawText(buttonRect2, Qt::AlignCenter,"Export");
            painter->drawText(buttonRect3, Qt::AlignCenter,"Update");
            break;
    }

    update();
}

void ButtonBar::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickedPos = event->buttonDownPos(Qt::LeftButton);

    qDebug() << clickedPos;

    switch(buttonDisplay) {
    case 1: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            emit sendAlarmAck();

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            buttonDisplay = 2;

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            buttonDisplay = 3;
        }

    case 2: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            buttonDisplay = 1;

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            // emit a signal to increase the fuel quantity
            emit sendFuelChange("+");

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            // emit a signal to decrease the fuel quantity
            emit sendFuelChange("-");
        }

    case 3: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            buttonDisplay = 1;

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            // emit a signal to show log file export dialog

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            // emit a signal to show a software update dialog
            QFile::copy("app.exe", "update.exe");
        }
    }

    QGraphicsItem::mousePressEvent(event);
}
