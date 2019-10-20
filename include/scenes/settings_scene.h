#ifndef SETTINGS_SCENE_H
#define SETTINGS_SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include "QDateTimeEdit"
#include "QProcess"
#include "QTextEdit"
#include "QString"

#include <userSettings.h>

class settingsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    settingsScene(QObject* parent = nullptr);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    userSettings userSet;
    QLabel timeLabel;
    QLabel dateLabel;
    QLabel hobbsLabel;
    QTime time;
    QString timeText;
    QString dateText;
    QString hobbsText;
    double hobbs;
    QTimer timer;
    QDateTimeEdit dtEdit;
    void setupNumPad();
    void setupSlider();
    QButtonGroup buttonGroup;
    QPushButton key1;
    QPushButton key2;
    QPushButton key3;
    QPushButton key4;
    QPushButton key5;
    QPushButton key6;
    QPushButton key7;
    QPushButton key8;
    QPushButton key9;
    QPushButton key0;
    QPushButton keyClr;
    QPushButton keyEnt;
    QPushButton keyColon;
    QPushButton keySlash;
    QGroupBox numPad;
    QGridLayout numLayout;

    QPushButton setDate;
    QPushButton setTime;
    QPushButton setHobbs;

    QTextEdit cmdLog;

    QProcess hwClock;
    QString execCommand;

    QSlider backlightSlider;

    void addSlashesOrColons();

    int editType = 0; // 0 - Not Editing; 1 - Date; 2 - Time

    QPushButton backlightBrighter;
    QPushButton backlightDimmer;
    QLabel backlightValue;
    void setupBacklight();
    QProcess backlightProc;

    QString stdout;
    QString stderr;

    QPushButton button1;

    QSettings settingsINI;

private slots:
    void onZeroCurrent() { emit zeroCurrent();}
    void showDateTime();
    void onFinishChange();
    void onChangeTime();
    void onChangeDate();
    void on1Pressed();
    void on2Pressed();
    void on3Pressed();
    void on4Pressed();
    void on5Pressed();
    void on6Pressed();
    void on7Pressed();
    void on8Pressed();
    void on9Pressed();
    void on0Pressed();
    void onClrPressed();
    void onChangeHobbs();
    void getHobbsFromINI();

signals:
    void zeroCurrent();
    void switchScene(int);
    void hobbsUpdated();

private slots:
    void onBacklightChange(int);
    void onButton1Pressed() {
        emit switchScene(1);
    }

};

#endif // SETTINGS_SCENE_H
