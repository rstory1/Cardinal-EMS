#include "instrumentsettings.h"

instrumentSettings::instrumentSettings(QGraphicsObject *parent) : QGraphicsObject(parent)
{

}

QRectF instrumentSettings::boundingRect() const
{
    return QRectF(0,0,800,480);
}

void instrumentSettings::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    //Save the painter and deactivate Antialising for rectangle drawing
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 0));
    painter->setFont(QFont("Arial", 14));
}

void instrumentSettings::onAdcUpdate(quint16 adcValue) {

}

void instrumentSettings::mousePressEvent(QGraphicsSceneMouseEvent *event) {

}
