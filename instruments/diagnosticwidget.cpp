#include "diagnosticwidget.h"

diagnosticWidget::diagnosticWidget(QGraphicsObject *parent)
    : QGraphicsObject(parent)
{

}

QRectF diagnosticWidget::boundingRect() const
{
    return QRectF(-45, -75, 85, 160);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void diagnosticWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //painter->setBrush(Qt::darkBlue);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 14, QFont::Bold));

    //Save thje painter and deactivate Antialising for rectangle drawing
    //painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    //painter->restore();
}
