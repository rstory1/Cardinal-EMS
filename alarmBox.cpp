#include "alarmBox.h"

AlarmBox::AlarmBox(QGraphicsObject *parent) : QGraphicsObject(parent)
{
//    wiringPiSetupSys();
//    pinMode(18, OUTPUT);
//    pinMode(17, INPUT);

//    wiringPiISR(17,INT_EDGE_FALLING, &buttonClicked);

//    textRect1 = QRectF(-202, -80, 202, 20);
}

QRectF AlarmBox::boundingRect() const
{
    return QRectF(-40, -100, 80, 200);
}

void AlarmBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);


    //Save thje painter and deactivate Antialising for rectangle drawing
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    //Define pen, brush and rect for the bar
    painter->setPen(Qt::white);
    //painter->setBrush(Qt::yellow); //This will fill in the rectangle
    painter->drawRect(QRectF(-40, -80, 80, 180));

    //Restore the painter with antialising
    painter->restore();

    painter->drawText(QRectF(-40, -100, 80, 20), Qt::AlignCenter, "ALARMS");

    if (alarmText[0] != "")
    {
        painter->setPen(alarmColor[0]);
        painter->setBrush(alarmColor[0]);

        painter->drawRect(QRectF(-38, -79, 76, 18));

    }
    painter->setFont(QFont("Arial", 12,true));
    painter->setPen(Qt::white);
    painter->drawText(QRectF(-40, -80, 80, 20), Qt::AlignCenter, alarmText[0]);


//    painter->setPen(alarmColor[1]);
//    painter->drawText(QRectF(QPointF(-80.0, -60.0), QPointF(60.0, -40.0)), Qt::AlignCenter, alarmText[1]);

//    painter->setPen(alarmColor[2]);
//    painter->drawText(QRectF(-80, -40, 120, 20), Qt::AlignCenter, alarmText[2]);

//    painter->setPen(alarmColor[3]);
//    painter->drawText(QRectF(-80, -20, 120, 20), Qt::AlignCenter, alarmText[3]);

//    painter->setPen(alarmColor[4]);
//    painter->drawText(QRectF(-80, 0, 120, 20), Qt::AlignCenter, alarmText[4]);

//    painter->setPen(alarmColor[5]);
//    painter->drawText(QRectF(-80, 20, 120, 20), Qt::AlignCenter, alarmText[5]);

//    painter->setPen(alarmColor[6]);
//    painter->drawText(QRectF(-80, 40, 120, 20), Qt::AlignCenter, alarmText[6]);

//    painter->setPen(alarmColor[7]);
//    painter->drawText(QRectF(-80, 60, 120, 20), Qt::AlignCenter, alarmText[7]);

//    painter->setPen(alarmColor[8]);
//    painter->drawText(QRectF(-80, 80, 120, 20), Qt::AlignCenter, alarmText[8]);

//    painter->setPen(alarmColor[9]);
//    painter->drawText(QRectF(-80, 100, 120, 20), Qt::AlignCenter, alarmText[9]);

    //  Update the item. Without this, the program will wait for something else to cause a redraw
    update();
}

void AlarmBox::soundAlarm(int alarmColor, int alarmSeverity, QString alarmText, QString alarmGauge)
{
    digitalWrite(18, HIGH);
}

void AlarmBox::buttonClicked()
{
    digitalWrite(18, LOW);
}

void AlarmBox::onRemoveAlarm(QString text)
{
    int removedItem;
    bool itemFound = false;

    for (int i = 0;i <= 9;i++) {
        if (itemFound == false) {
            if ( alarmText[i] == text ) {
                itemFound = true;

                removedItem = i;

                alarmText[removedItem] = "";

                for (int a = removedItem + 1;a<=alarmCount;a++) {
                    alarmText[a-1] = alarmText[a];
                    alarmColor[a-1] =  alarmColor[a];
                }

                alarmCount = alarmCount - 1;
            }
        }

    }
}

void AlarmBox::onAlarm(QString text, QColor color, bool flashing)
{
    alarmText[alarmCount] = text;
    alarmColor[alarmCount] = color;

    alarmCount++;
}
