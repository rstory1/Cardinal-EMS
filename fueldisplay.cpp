#include "fueldisplay.h"

FuelDisplay::FuelDisplay(QGraphicsObject *parent)
    : QGraphicsObject(parent)
    , settings("./settings.ini", QSettings::IniFormat)
    , fuelAmount(0.0)
    , fuelFlow(0.0)
    , timeToDestination(1.0)
    , remainingFuelRect(-95, -20, 90, 55)
    , remainingFuelAtDestinationRect(-95, -80, 90, 55)
    , mpgRect(0, -80, 90, 55)
    , rangeRect(0, -20, 90, 55)
{
    fuelAmount = settings.value("Fueling/LastShutdown", 0.0).toDouble();
    fuelUnits = settings.value("Units/fuel;", "gal").toString();
}

QRectF FuelDisplay::boundingRect() const
{
    return QRectF(-100, -100, 200, 200);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void FuelDisplay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    double fuelAtDestination = fuelAmount - (fuelFlow * timeToDestination);

    // This is purely for testing.
    double airspeed = 100;

    //Save thje painter and deactivate Antialising for rectangle drawing
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    //Draw fuel display
    painter->setBrush(Qt::black);
    painter->setPen(Qt::white);
    painter->drawRect(boundingRect());

    painter->drawRoundedRect(remainingFuelRect, 5, 5);
    painter->drawRoundedRect(remainingFuelAtDestinationRect, 5, 5);
    painter->drawRoundedRect(mpgRect, 5, 5);
    painter->drawRoundedRect(rangeRect, 5, 5);
    //painter->drawRoundedRect(homeRect, 5, 5);

    painter->restore();

    // Draw headings for values
    painter->setPen(Qt::gray);
    painter->drawText(QRectF(-100,-95,200,10), Qt::AlignVCenter | Qt::AlignCenter, "FUEL (gal)");

    painter->drawText(QRectF(remainingFuelRect.left(), remainingFuelRect.top() + 5, remainingFuelRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "Fuel\nLevel");
    painter->drawText(QRectF(remainingFuelAtDestinationRect.left(), remainingFuelAtDestinationRect.top() + 5, remainingFuelAtDestinationRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "Fuel at\nDestination");
    painter->drawText(QRectF(mpgRect.left(), mpgRect.top() + 5, mpgRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "NMPG");
    painter->drawText(QRectF(rangeRect.left(), rangeRect.top() + 5, rangeRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "Range\n(hrs)");

    // Draw values
    painter->setFont(QFont("Arial", 18, QFont::Bold));
    painter->setPen(Qt::white);
    painter->drawText(QRectF(remainingFuelRect.left(), remainingFuelRect.top() + 35, remainingFuelRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, QString::number(fuelAmount, 'f', 1));
    painter->drawText(QRectF(remainingFuelAtDestinationRect.left(), remainingFuelAtDestinationRect.top() + 35, remainingFuelAtDestinationRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, QString::number(fuelAtDestination, 'f', 1));
    painter->drawText(QRectF(mpgRect.left(), mpgRect.top() + 35, mpgRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, QString::number(airspeed/fuelFlow, 'f', 1));
    painter->drawText(QRectF(rangeRect.left(), rangeRect.top() + 35, rangeRect.width(), 18), Qt::AlignVCenter | Qt::AlignCenter, QString::number(fuelAmount/fuelFlow,'f',1));
}

void FuelDisplay::setFuelFlow(double value)
{
    fuelFlow = value;
}

void FuelDisplay::setTimeToDestination(double time)
{
    timeToDestination = time;
}

void FuelDisplay::onFuelAmountChange(QString changeDirection) {
    if (changeDirection=="+") {
        fuelAmount++;
    } else {
        fuelAmount--;
    }
}
