#include "scenes/settings_scene.h"

settingsScene::settingsScene(QObject* parent) :
    QGraphicsScene(parent)
{
    //addItem(&userSet);

    setupNumPad();

    //setBackgroundBrush(Qt::black);

    label.setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    label.setGeometry(10,10,100,20);
    addWidget(&label);

    connect(&userSet, SIGNAL(zeroCurrent()), this, SLOT(onZeroCurrent()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer.start(1000);

    setTime.setText("Change Time");
    setTime.setFixedHeight(40);
    setTime.setGeometry(110,10,100,40);
    addWidget(&setTime);

    setDate.setText("Change Date");
    setDate.setFixedHeight(40);
    setDate.setGeometry(110,50,100,40);
    addWidget(&setDate);

    connect(&keyEnt, SIGNAL(clicked(bool)), this, SLOT(onFinishChange()));
    connect(&setTime, SIGNAL(clicked(bool)), this, SLOT(onChangeTime()));
    connect(&setDate, SIGNAL(clicked(bool)), this, SLOT(onChangeDate()));

    connect(&key1, SIGNAL(clicked(bool)), this, SLOT(on1Pressed()));
    connect(&key2, SIGNAL(clicked(bool)), this, SLOT(on2Pressed()));
    connect(&key3, SIGNAL(clicked(bool)), this, SLOT(on3Pressed()));
    connect(&key4, SIGNAL(clicked(bool)), this, SLOT(on4Pressed()));
    connect(&key5, SIGNAL(clicked(bool)), this, SLOT(on5Pressed()));
    connect(&key6, SIGNAL(clicked(bool)), this, SLOT(on6Pressed()));
    connect(&key7, SIGNAL(clicked(bool)), this, SLOT(on7Pressed()));
    connect(&key8, SIGNAL(clicked(bool)), this, SLOT(on8Pressed()));
    connect(&key9, SIGNAL(clicked(bool)), this, SLOT(on9Pressed()));
    connect(&key0, SIGNAL(clicked(bool)), this, SLOT(on0Pressed()));

    cmdLog.setFixedSize(300,350);
    cmdLog.setGeometry(10,40,300,350);
    addWidget(&cmdLog);
}

QRectF settingsScene::boundingRect() const
{
    return QRectF(0,0,800,480);
}

void settingsScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    if(isActive()) {
        update();
    }

}

void settingsScene::showTime() {
    time = QTime::currentTime();

    if(!keyColon.isVisible()) {
        timeText = time.toString("hh:mm:ss");
    }

    label.setText(timeText);
}

void settingsScene::setupNumPad() {

    key1.setText("1");
    key2.setText("2");
    key3.setText("3");
    key4.setText("4");
    key5.setText("5");
    key6.setText("6");
    key7.setText("7");
    key8.setText("8");
    key9.setText("9");
    keyClr.setText("Clr");
    key0.setText("0");
    keyEnt.setText("Ent");
    keyColon.setText(":");
    keySlash.setText("/");

    int hwSize = 50;
    int hwSize2 = hwSize + hwSize;
    int hwSize3 = hwSize2 + hwSize;
    int initX = 220;
    int initY = 20;

    key1.setFixedSize(hwSize,hwSize);
    key2.setFixedSize(hwSize,hwSize);
    key3.setFixedSize(hwSize,hwSize);
    key4.setFixedSize(hwSize,hwSize);
    key5.setFixedSize(hwSize,hwSize);
    key6.setFixedSize(hwSize,hwSize);
    key7.setFixedSize(hwSize,hwSize);
    key8.setFixedSize(hwSize,hwSize);
    key9.setFixedSize(hwSize,hwSize);
    keyClr.setFixedSize(hwSize,hwSize);
    key0.setFixedSize(hwSize,hwSize);
    keyEnt.setFixedSize(hwSize,hwSize);
    keyColon.setFixedSize(hwSize,hwSize3);
    keySlash.setFixedSize(hwSize,hwSize3);

    key1.setGeometry(initX, initY, hwSize, hwSize);
    key2.setGeometry(initX+hwSize, initY, hwSize, hwSize);
    key3.setGeometry(initX+hwSize2, initY, hwSize, hwSize);

    key4.setGeometry(initX, initY+hwSize, hwSize, hwSize);
    key5.setGeometry(initX+hwSize, initY+hwSize, hwSize, hwSize);
    key6.setGeometry(initX+hwSize2, initY+hwSize, hwSize, hwSize);

    key7.setGeometry(initX, initY+hwSize2, hwSize, hwSize);
    key8.setGeometry(initX+hwSize, initY+hwSize2, hwSize, hwSize);
    key9.setGeometry(initX+hwSize2, initY+hwSize2, hwSize, hwSize);

    keyClr.setGeometry(initX, initY+hwSize3, hwSize, hwSize);
    key0.setGeometry(initX+hwSize, initY+hwSize3, hwSize, hwSize);
    keyEnt.setGeometry(initX+hwSize2, initY+hwSize3, hwSize, hwSize);

    keyColon.setGeometry(initX+hwSize3, initY, hwSize, hwSize3);
    keySlash.setGeometry(initX+hwSize3, initY, hwSize, hwSize3);

    key1.setVisible(false);
    key2.setVisible(false);
    key3.setVisible(false);
    key4.setVisible(false);
    key5.setVisible(false);
    key6.setVisible(false);
    key7.setVisible(false);
    key8.setVisible(false);
    key9.setVisible(false);
    keyClr.setVisible(false);
    key0.setVisible(false);
    keyEnt.setVisible(false);
    keyColon.setVisible(false);
    keySlash.setVisible(false);

    addWidget(&key1);
    addWidget(&key2);
    addWidget(&key3);
    addWidget(&key4);
    addWidget(&key5);
    addWidget(&key6);
    addWidget(&key7);
    addWidget(&key8);
    addWidget(&key9);
    addWidget(&keyClr);
    addWidget(&key0);
    addWidget(&keyEnt);
    addWidget(&keyColon);
    addWidget(&keySlash);

}

void settingsScene::onChangeTime() {
    key1.setVisible(true);
    key2.setVisible(true);
    key3.setVisible(true);
    key4.setVisible(true);
    key5.setVisible(true);
    key6.setVisible(true);
    key7.setVisible(true);
    key8.setVisible(true);
    key9.setVisible(true);
    keyClr.setVisible(true);
    key0.setVisible(true);
    keyEnt.setVisible(true);
    keyColon.setVisible(true);
    keySlash.setVisible(false);

    timeText.clear();
}

void settingsScene::onChangeDate() {
    key1.setVisible(true);
    key2.setVisible(true);
    key3.setVisible(true);
    key4.setVisible(true);
    key5.setVisible(true);
    key6.setVisible(true);
    key7.setVisible(true);
    key8.setVisible(true);
    key9.setVisible(true);
    keyClr.setVisible(true);
    key0.setVisible(true);
    keyEnt.setVisible(true);
    keyColon.setVisible(false);
    keySlash.setVisible(true);
}

void settingsScene::onFinishChange() {
    key1.setVisible(false);
    key2.setVisible(false);
    key3.setVisible(false);
    key4.setVisible(false);
    key5.setVisible(false);
    key6.setVisible(false);
    key7.setVisible(false);
    key8.setVisible(false);
    key9.setVisible(false);
    keyClr.setVisible(false);
    key0.setVisible(false);
    keyEnt.setVisible(false);
    keyColon.setVisible(false);
    keySlash.setVisible(false);

    QString dateTimeStr = "\"" + QDate::currentDate().toString("dd MMM yyyy") + " " + timeText + "\"";

    qDebug() << dateTimeStr;
    QString execCommand = "date -s " + dateTimeStr;
    QString execCommand2 = "hwclock -w";

    qDebug() << execCommand;
    QProcess hwClock;
    hwClock.start(execCommand);
    hwClock.waitForFinished(-1); // will wait forever until finished

    QString stdout = hwClock.readAllStandardOutput();
    QString stderr = hwClock.readAllStandardError();
    cmdLog.append(stdout);
    cmdLog.append(stderr);

    hwClock.start(execCommand2);
    hwClock.waitForFinished(-1); // will wait forever until finished

    stdout = hwClock.readAllStandardOutput();
    stderr = hwClock.readAllStandardError();
    cmdLog.append(stdout);
    cmdLog.append(stderr);
}

void settingsScene::addColons() {
    int timeLength = timeText.length();

    if (timeLength==2 || timeLength == 5) {
        timeText.append(":");
    } else if (timeLength == 8) {
        timeText.clear();
    }
}

void settingsScene::on1Pressed() {
    addColons();

    timeText.append("1");
}

void settingsScene::on2Pressed() {
    addColons();
    timeText.append("2");
}

void settingsScene::on3Pressed() {
    addColons();

    timeText.append("3");
}

void settingsScene::on4Pressed() {
    addColons();
    timeText.append("4");
}

void settingsScene::on5Pressed() {
    addColons();
    timeText.append("5");
}

void settingsScene::on6Pressed() {
    addColons();
    timeText.append("6");
}

void settingsScene::on7Pressed() {
    addColons();
    timeText.append("7");
}

void settingsScene::on8Pressed() {
    addColons();
    timeText.append("8");
}

void settingsScene::on9Pressed() {
    addColons();
    timeText.append("9");
}

void settingsScene::on0Pressed() {
    addColons();
    timeText.append("0");
}
