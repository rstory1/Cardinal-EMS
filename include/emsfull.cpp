#include "emsfull.h"

emsFull::emsFull(QGraphicsObject *parent)
    :  QGraphicsObject(parent)
{

}

QRectF emsFull::boundingRect() const
{
    return QRectF(-400,-240,800,480);
}

void emsFull::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    //Save the painter and deactivate Antialising for rectangle drawing
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 0));
    painter->setFont(QFont("Arial", 14));

    update();

}
