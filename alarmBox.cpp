#include "alarmBox.h"

AlarmBox::AlarmBox(QGraphicsObject *parent) : QGraphicsObject(parent)
{
//    wiringPiSetupSys();
//    pinMode(18, OUTPUT);
//    pinMode(17, INPUT);

//    wiringPiISR(17,INT_EDGE_FALLING, &buttonClicked);

//    QTimer *flashTimer = new QTimer(this);
//        connect(flashTimer, SIGNAL(timeout()), this, SLOT(changeFlashState()));
//        flashTimer->start(1000);
}

QRectF AlarmBox::boundingRect() const
{
    return QRectF(boundingX, boundingY, boundingWidth, boundingHeight);
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
    painter->drawRect(QRectF(boundingX, boundingY + 20, boundingWidth, boundingHeight - 20));

    for (int i=0;i<=9;i++) {
        if (alarmText[i] != "")
        {
            if ((flashState == true && alarmFlash[i] == true) || alarmFlash[i] == false) {
                painter->setPen(alarmColor[i]);
                painter->setBrush(alarmColor[i]);

                painter->drawRect(QRectF(boundingX + 2, boundingY + 21 + (20 * i), boundingWidth - 4, 18));
            }
        }
    }

    //Restore the painter with antialising
    painter->restore();

    painter->setPen(Qt::white);
    painter->drawText(QRectF(-40, -100, 80, 20), Qt::AlignCenter, "ALARMS");

    QFont font = QFont("Arial", 12);
    font.setBold(true);
    painter->setFont(font);

    for (int i=0;i<=9;i++) {
        if (alarmText[i] != "")
        {
            if (flashState == true) {
                if (alarmColor[i] == Qt::yellow) {
                    painter->setPen(Qt::black);
                } else {
                    painter->setPen(Qt::white);
                }
            } else {
                painter->setPen(alarmColor[i]);
            }

            painter->drawText(QRectF(boundingX + 2, boundingY + 21 + (20 * i), boundingWidth - 4, 18), Qt::AlignCenter, alarmText[i]);
        }
    }

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
    alarmFlash[alarmCount] = flashing;

    alarmCount++;

    QSound::play("alarm.wav");

}

void AlarmBox::changeFlashState()
{
    if (flashState == false) {
        flashState  = true;
    } else {
        flashState = false;
    }
}
