#include "fueldisplay.h"

FuelDisplay::FuelDisplay(QGraphicsObject *parent)
    : QGraphicsObject(parent)
    , settings("./settings.ini", QSettings::IniFormat)
    , fuelAmount(0.0)
    , fuelFlow(0.0)
    , timeToDestination(1.0)
    , remainingFuelRect(-95, -20, 90, 52)
    , remainingFuelAtDestinationRect(-95, -75, 90, 52)
{
    fuelAmount = settings.value("Fueling/LastShutdown", 0.0).toDouble();
    fuelUnits = settings.value("Units/fuel;", "gal").toString();
}

QRectF FuelDisplay::boundingRect() const
{
    return QRectF(-100, -100, 200, 200);
}

void FuelDisplay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Save thje painter and deactivate Antialising for rectangle drawing
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPen edgePen(Qt::transparent, 0);
    double fuelAtDestination = fuelAmount - (fuelFlow * timeToDestination);

    //Draw fuel display
    painter->setBrush(Qt::black);
    painter->setPen(Qt::gray);
    painter->drawRect(boundingRect());

    painter->setBrush(Qt::black);
    painter->setPen(Qt::gray);
    painter->drawRoundedRect(remainingFuelRect, 5, 5);
    painter->drawRoundedRect(remainingFuelAtDestinationRect, 5, 5);
    //painter->drawRoundedRect(fuelFlowRect, 5, 5);
    //painter->drawRoundedRect(fuelingRect, 5, 5);
    //painter->drawRoundedRect(homeRect, 5, 5);

    painter->restore();

    painter->setPen(Qt::gray);
    painter->drawText(QRectF(-100,-95,200,10), Qt::AlignVCenter | Qt::AlignCenter, "FUEL");

//    painter->setPen(Qt::white);
    painter->drawText(QRectF(remainingFuelRect.left(), remainingFuelRect.top() + 5, remainingFuelRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "Fuel\nRemaining");
    painter->drawText(QRectF(remainingFuelAtDestinationRect.left(), remainingFuelAtDestinationRect.top() + 5, remainingFuelAtDestinationRect.width(), 30), Qt::AlignVCenter | Qt::AlignCenter, "Fuel at\nDestination");
//    //painter->drawText(fuelFlowRect, Qt::AlignVCenter | Qt::AlignLeft, " Fuel flow:");

    painter->setPen(Qt::white);
    painter->drawText(QRectF(remainingFuelRect.left(), remainingFuelRect.top() + 35, remainingFuelRect.width(), 12), Qt::AlignVCenter | Qt::AlignCenter, QString::number(fuelAmount, 'f', 1));
    painter->drawText(QRectF(remainingFuelAtDestinationRect.left(), remainingFuelAtDestinationRect.top() + 35, remainingFuelAtDestinationRect.width(), 12), Qt::AlignVCenter | Qt::AlignCenter, QString::number(fuelAtDestination, 'f', 1));
    //painter->drawText(fuelFlowRect, Qt::AlignVCenter | Qt::AlignRight, QString::number(fuelFlow, 'f', 1).append(QString(" %1 ").arg(settings.value("Units/fuel").toString()).toLatin1()));

    //painter->drawText(fuelingRect, Qt::AlignCenter, "Fueling");
    //painter->drawText(homeRect, Qt::AlignCenter, "Home");
}

void FuelDisplay::setFuelFlow(double value)
{
    fuelFlow = value;
}

void FuelDisplay::setTimeToDestination(double time)
{
    timeToDestination = time;
}
