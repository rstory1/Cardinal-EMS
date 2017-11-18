#include "windvector.h"

WindVector::WindVector(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    vector1 << QPointF(-2,20) << QPointF(2,20) << QPointF(2,-12) << QPointF(10,-12) << QPointF(0,-27) << QPointF(-10,-12) << QPointF(-2,-12) << QPointF(-2,20);
    vector2 << vector1;
    vector3 << vector1;
    spdTextRect = QRectF(-55, -80, 100, 100);
    xCompRect = QRectF(25, -7.5, 25, 15);
    yCompRect = QRectF(-10, -40, 25, 15);

    //isVector = false;
}

QRectF WindVector::boundingRect() const
{
    return QRectF(-50, -50, 100, 100);
}

void WindVector::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    //Save thje painter and deactivate Antialising for rectangle drawing
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::black, 0));
    painter->setFont(QFont("Arial", 14));

    if (isVector) {
        painter->drawPolygon(vector1);

        //QString test = ;
        painter->setPen(QPen(Qt::white, 0));
        painter->drawText(spdTextRect, Qt::AlignCenter | Qt::AlignVCenter, QString::number((windDir + magHdg)-180,'f',0).rightJustified(3,'0').append(QString("/")).append(QString::number(windSpd, 'f', 0)));
    } else {
        painter->drawPolygon(vector2);
        painter->drawPolygon(vector3);

        painter->setPen(QPen(Qt::white, 0));
        painter->drawText(xCompRect, Qt::AlignCenter | Qt::AlignVCenter, QString::number(windX, 'f', 0));
        painter->drawText(yCompRect, Qt::AlignCenter | Qt::AlignVCenter, QString::number(windY, 'f', 0));
    }

}

void WindVector::updateWind(float spd, float dir, float mHdg) {
    float windDirTo;
    if (dir > 180) {
        windDirTo = dir - 180 - mHdg;
        if ((windDirTo-mHdg) != windDir) {
            windDir = windDirTo - windDir;
            magHdg = mHdg;
            t.rotate(windDir);

            // Calculate wind components relative to airplane.
            windX = spd * cos((windDir)*(PI/180.0));
            windY = spd * sin((windDir)*(PI/180.0));
        }
    } else {
        windDirTo = dir + 180 - mHdg;
        if (windDirTo != windDir) {
            windDir = windDirTo - windDir;
            magHdg = mHdg;

            // Calculate wind components relative to airplane.
            windX = spd * cos((windDir)*(PI/180.0));
            windY = spd * sin((windDir)*(PI/180.0));

            if (isVector) {
                t.rotate(windDir);
                vector1 = t.map(vector1);
            } else {
                if (windY <0) {
                    t.rotate(180);
                    vector2 = t.map(vector2);
                    windY = (-1) * windY;
                }

                if (windX < 0) {
                    t.rotate(90);
                    vector3 = t.map(vector3);
                    windX = (-1) * windX;
                }
            }


        }
    }

    windSpd = spd;

    update();
}
