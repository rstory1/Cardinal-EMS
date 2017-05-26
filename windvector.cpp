#include "windvector.h"

WindVector::WindVector(QGraphicsObject *parent) : QGraphicsObject(parent)
{
    vector << QPointF(-5,20) << QPointF(5,20) << QPointF(5,-5) << QPointF(10,-5) << QPointF(0,-20) << QPointF(-10,-5) << QPointF(-5,-5);
    spdTextRect = QRectF(-50, -80, 100, 100);
}

QRectF WindVector::boundingRect() const
{
    return QRectF(-100, -100, 200, 200);
}

void WindVector::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    painter->setBrush(Qt::white);
    painter->setPen(QPen(Qt::white, 0));

    vector2 = t.map(vector);

    painter->drawPolygon(vector2);

    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 14));

    QString test = QString::number((windDir + magHdg)-180,'f',0).rightJustified(3,'0');
    painter->drawText(spdTextRect, Qt::AlignCenter | Qt::AlignVCenter, test.append(QString("/")).append(QString::number(windSpd, 'f', 0)));

}

void WindVector::updateWind(float spd, float dir, float mHdg) {
    float windDirTo;
    if (dir > 180) {
        windDirTo = dir - 180 - mHdg;
        if ((windDirTo-mHdg) != windDir) {
            windDir = windDirTo - windDir;
            magHdg = mHdg;
            t.rotate(windDir);
        }
    } else {
        windDirTo = dir + 180 - mHdg;
        if (windDirTo != windDir) {
            windDir = windDirTo - windDir;
            magHdg = mHdg;
            t.rotate(windDir);
        }
    }

    // Calculate wind components relative to airplane.
    windX = spd * cos((windDir)*(PI/180.0));
    windY = spd * sin((windDir)*(PI/180.0));

    windSpd = spd;

    update();
}
