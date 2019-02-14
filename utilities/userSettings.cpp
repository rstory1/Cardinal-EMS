#include "userSettings.h"

userSettings::userSettings(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    backgroundRect = QRectF(0,0,800,480);

//    oilTbox = QRectF(20,20,170,50);

//    oilPbox = QRectF(200,20,170,50);

//    manPbox = QRectF(20,80,170,50);

//    egtChtBox = QRectF(200,80,170,50);

    currentBox = QRectF(20,20,170,50);

//    ffBox = QRectF(200,140,170,50);
}

QRectF userSettings::boundingRect() const
{
    return QRectF(0,0,800,480);
}

void userSettings::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    //Save the painter and deactivate Antialising for rectangle drawing
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 0));
    painter->setFont(QFont("Arial", 14));

    painter->drawRect(backgroundRect);

    //painter->drawPolygon(vector1);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(Qt::white, 1));

//    painter->drawRect(oilTbox);
//    painter->drawRect(oilPbox);
//    painter->drawRect(manPbox);
//    painter->drawRect(egtChtBox);
//    painter->drawRect(ffBox);
    painter->drawRoundedRect(currentBox, 10, 10);

    //painter->drawText(oilTbox, QString("  Oil Temp"), QTextOption(Qt::AlignVCenter));

    //painter->drawText(oilPbox, QString("  Press"), QTextOption(Qt::AlignVCenter));

    //painter->drawText(manPbox, QString("  Man Press"), QTextOption(Qt::AlignVCenter));

    //painter->drawText(egtChtBox, QString("  EGT/CHT"), QTextOption(Qt::AlignVCenter));

    //painter->drawText(ffBox, QString("  Fuel Flow"), QTextOption(Qt::AlignVCenter));

    painter->drawText(currentBox, QString("Current"), QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));

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

void userSettings::mousePressEvent(QGraphicsSceneMouseEvent *event) {

    QPointF position = event->pos();

    if ((position.x() > currentBox.x() && position.x() < currentBox.x() + currentBox.width()) && (position.y() > currentBox.y())) {
        emit zeroCurrent();
    }
}
