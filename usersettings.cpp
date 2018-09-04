#include "usersettings.h"

userSettings::userSettings(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    backgroundRect = QRectF(-400,-240,800,480);


    oilTbox = QRectF(-380,-220,170,50);
    oilTcheckbox = QRectF(-240,-205,20,20);

    oilPbox = QRectF(-200,-220,100,50);


    manPbox = QRectF(-380,-160,100,50);


    egtBox = QRectF(-200,-160,100,50);


    chtBox = QRectF(-380,-100,100,50);


    ffBox = QRectF(-200,-100,100,50);



}

QRectF userSettings::boundingRect() const
{
    return QRectF(-400,-240,800,480);
}

void userSettings::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    //Save thje painter and deactivate Antialising for rectangle drawing
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 0));
    painter->setFont(QFont("Arial", 14));

    painter->drawRect(backgroundRect);

    //painter->drawPolygon(vector1);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::green, 1));

    painter->drawRect(oilTbox);
    painter->drawRect(oilPbox);
    painter->drawRect(manPbox);
    painter->drawRect(egtBox);
    painter->drawRect(chtBox);
    painter->drawRect(ffBox);

    painter->drawText(oilTbox, QString("  Hide Oil Temp"), QTextOption(Qt::AlignVCenter));
    painter->drawRect(oilTcheckbox);
    painter->drawText(oilTcheckbox, QString("X"), QTextOption(Qt::AlignVCenter | Qt::AlignHCenter));

    //painter->drawText("Hello", Qt::AlignCenter | Qt::AlignVCenter);

    update();

}

void userSettings::showSettings(bool tfShow) {
    if (tfShow) {
        this->show();
    } else {
        this->hide();
    }
}
