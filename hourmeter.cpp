#include "hourmeter.h"

HourMeter::HourMeter(QGraphicsObject *parent) : QGraphicsObject(parent), settings(":/settings.ini", QSettings::IniFormat, parent)
{
    hobbs.hour = 0;
    hobbs.min = 0;
    hobbs.sec = 0;

    flight.hour = 0;
    flight.min = 0;
    flight.sec = 0;

    hobbsString = settings.value("Time/Hobbs", "00:00:00").toString();
}

QRectF HourMeter::boundingRect() const
{
    return QRectF(-50, -25, 100, 85);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void HourMeter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //Save thje painter and deactivate Antialising for rectangle drawing
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    //Define pen, brush and rect for the bar
    painter->setPen(Qt::gray);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(boundingRect());

    //Restore the painter with antialising
    painter->restore();

    //Draw Texts around (title, min and max value)
    painter->setPen(Qt::white);
    painter->drawText(QRectF(-75, -20, 150, 15), Qt::AlignCenter | Qt::AlignVCenter,QString("Hobbs"));
    painter->drawText(QRectF(-75, 20, 150, 15), Qt::AlignCenter | Qt::AlignVCenter,QString("Flight"));
    //painter->drawText(QRectF(-25, -10, 50, 10), Qt::AlignCenter | Qt::AlignVCenter,unitText);
    painter->setPen(Qt::white);

    //Draw readout
    font.setBold(true);
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(QRectF(-75, 0, 150, 15), Qt::AlignCenter | Qt::AlignVCenter, hobbsString);
    painter->drawText(QRectF(-75, 40, 150, 15), Qt::AlignCenter | Qt::AlignVCenter, hobbsString);
}

void HourMeter::onTic(/*bool isFlying*/) {
    bool isFlying = false;
    if (hobbs.sec < 59) {
        hobbs.sec = hobbs.sec + 1;
    } else {
        hobbs.sec = 0;

        if (hobbs.min < 59) {
            hobbs.min = hobbs.min + 1;
            if (hobbs.min == 6) {
                settings.value("Time/hobbs") = hobbsString;
            }
        } else {
            hobbs.min = 0;
            hobbs.hour = hobbs.hour + 1;
        }
    }

    if (isFlying) {
        if (flight.sec < 59) {
            flight.sec = flight.sec + 1;
        } else {
            flight.sec = 0;

            if (flight.min < 59) {
                flight.min = flight.min + 1;
            } else {
                flight.min = 0;
                flight.hour = flight.hour + 1;
            }
        }
    }

    hobbsString = QString::number(hobbs.hour, 'f', 0).rightJustified(2,'0').append(QString(":").append(QString::number(hobbs.min, 'f',0).rightJustified(2,'0'))).append(QString(":").append(QString::number(hobbs.sec, 'f',0).rightJustified(2,'0')));
    //qDebug() << hobbsString;

    update();
}
