#ifndef ALARMBOX_H
#define ALARMBOX_H

#include <QString>
//#include <wiringPi.h>
#include <QtCore>
#include <QtWidgets>
#include <QtMultimedia/QSoundEffect>
#include <QtMultimedia/QSound>
#include <QtTextToSpeech/QTextToSpeech>

class AlarmBox : public QGraphicsObject
{
    Q_OBJECT

private:
    quint64 counter = 1;
    QString alarmText[10];
    QColor alarmColor[10];
    bool alarmFlash[10];
    int alarmCount = 0;
    QRectF textRect1;
    QString textForAlarm;
    int boundingX = -50;
    int boundingY = -100;
    int boundingWidth = 100;
    int boundingHeight = 200;
    bool flashState = false;

public:
    explicit AlarmBox(QGraphicsObject * parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void soundAlarm(int alarmColor, int alarmSeverity, QString alarmText, QString alarmGauge);
    void clearRpmAlarm();
    static void buttonClicked();
    void hideAlarmBox();
    void setText(QString text);

public slots:
    void onAlarm(QString text, QColor color, bool flashing);
    void onRemoveAlarm(QString text);
    void changeFlashState();
    void onAlarmAck();

};

#endif // ALARMBOX_H
