#include "backlight.h"

backlight::backlight(QGraphicsObject *parent) : QGraphicsObject(parent)
{

}

/*! \brief Defines a rectangle that is used to determine what should be redrawn during the update
*/
QRectF backlight::boundingRect() const
{
    return QRectF(boundingX, boundingY, boundingWidth, boundingHeight);
}

void backlight::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Save the painter and deactivate Antialising for rectangle drawing
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    //Define pen, brush and rect for the bar
    painter->setPen(Qt::white);
    painter->drawRect(QRectF(boundingX, boundingY + 20, boundingWidth, boundingHeight - 20));



    //Restore the painter with antialising
    painter->restore();

    //  Update the item. Without this, the program will wait for something else to cause a redraw
    update();
}

void backlight::setBrightness(int brightnessPercent) {
    std::ofstream file("/sys/class/backlight/backlight/brightness");
    if (!file.is_open())
        throw std::runtime_error("Could not open the file");
    file << brightnessPercent;
    file.close();
}
