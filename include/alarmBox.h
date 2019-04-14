//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Cardinal EMS, Experimental Aircraft EMS                              //
// Copyright (C) 2017 Ryan Story                                        //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ALARMBOX_H
#define ALARMBOX_H

#include <QString>
//#include <wiringPi.h>
#include <QtCore>
#include <QtWidgets>
//#include <QtMultimedia/QSoundEffect>
//#include <QtMultimedia/QSound>
//#include <QtTextToSpeech/QTextToSpeech>

//! Alarm Box Class
/*!
 * This class creates a box to display alarms created from the gauge instances and show the user.
*/

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
    int boundingHeight = 150;

public:
    explicit AlarmBox(QGraphicsObject * parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void soundAlarm(int alarmColor, int alarmSeverity, QString alarmText, QString alarmGauge);
    void clearRpmAlarm();
    static void buttonClicked();
    void hideAlarmBox();
    void setText(QString text);
    bool flashState = false;

public slots:
    void onAlarm(QString text, QColor color, bool flashing);
    void onRemoveAlarm(QString text);
    void changeFlashState();
    void onAlarmAck();

signals:
    void flashingAlarm();
    void stopAlarmFlash();

};

#endif // ALARMBOX_H
