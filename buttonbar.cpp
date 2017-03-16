#include "buttonbar.h"

ButtonBar::ButtonBar(QGraphicsObject *parent) : QGraphicsObject(parent)
{
}

QRectF ButtonBar::boundingRect() const
{
    return QRectF(0.0, -50.0, 600.0, 50.0);
}


void ButtonBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::darkBlue);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 14, QFont::Bold));

    QRectF buttonRect1 = QRectF(0,-50.0,100,50);
    QRectF buttonRect2 = QRectF(100,-50.0,100,50);
    QRectF buttonRect3 = QRectF(200,-50.0,100,50);
    QRectF buttonRect4 = QRectF(300,-50.0,100,50);
    QRectF buttonRect5 = QRectF(400,-50.0,100,50);
    QRectF buttonRect6 = QRectF(500,-50.0,100,50);

    //Save thje painter and deactivate Antialising for rectangle drawing
    //painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    //painter->restore();



    switch(buttonDisplay) {
    case 1: painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);

            painter->drawText(buttonRect1, Qt::AlignCenter,"Ack");
            painter->drawText(buttonRect2, Qt::AlignCenter,"Fuel");
            painter->drawText(buttonRect3, Qt::AlignCenter,"Settings");
            break;

    case 2: painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);
            painter->drawRect(buttonRect4);
            painter->drawRect(buttonRect5);
            painter->drawRect(buttonRect6);

            painter->drawText(buttonRect2, Qt::AlignCenter,"Full");
            painter->drawText(buttonRect3, Qt::AlignCenter,"+1");
            painter->drawText(buttonRect4, Qt::AlignCenter,"+5");
            painter->drawText(buttonRect5, Qt::AlignCenter,"-1");
            painter->drawText(buttonRect6, Qt::AlignCenter,"-5");
            break;
    }

    update();
}
