#include "scenes/emsfull.h"

emsFull::emsFull(QObject *parent)
    :  QGraphicsScene(parent)
    , gaugeSettings(QCoreApplication::applicationDirPath() + "/ems/settings/gaugeSettings.ini", QSettings::IniFormat, parent)
    , settings(QCoreApplication::applicationDirPath() + "/ems/settings/settings.ini", QSettings::IniFormat, parent)
{
    //Setting up the items to be displayed
    setupRpmIndicator();
    setupBarGraphs();
    setupManifoldPressure();
    setupChtEgt();
    setupFuelManagement();
    //setupWindVector();
    setupHourMeter();

    setSceneRect(0,0,800,480);

    setBackgroundBrush(Qt::black);

    connectSignals();

    // Initialize the timer to flash values on alarm
    flashTimer.start(1000);

    clockTimer.start(1000);

    button1.setText("Settings");
    button1.setFixedSize(100,50);
    button1.setGeometry(0,430,100,50);
    button1.setStyleSheet("QPushButton {\
                              background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0.7 black, stop: 1 darkGray);\
                                color: white;\
                                border:1px solid white;}");
    addWidget(&button1);

    button2.setText("Ack");
    button2.setFixedSize(100,50);
    button2.setGeometry(100,430,100,50);
    button2.setStyleSheet("QPushButton {\
                              background-color: qlineargradient(x1: 0, y1: 1, x2: 0, y2: 0,stop: 0.7 black, stop: 1 darkGray);\
                                color: white;\
                                border:1px solid white;}");
    addWidget(&button2);
    button2.setVisible(false);

    //Demo timer, for testing purposes only
#ifdef QT_DEBUG
    QTimer *demoTimer = new QTimer(this);
    connect(demoTimer, SIGNAL(timeout()), this, SLOT(demoFunction()));
    demoTimer->setSingleShot(false);
    demoTimer->start(200);
#endif

}

QRectF emsFull::boundingRect() const
{
    return QRectF(-400,-240,800,480);
}

void emsFull::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    //Save the painter and deactivate Antialising for rectangle drawing
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    painter->setBrush(Qt::black);
    painter->setPen(QPen(Qt::black, 0));
    painter->setFont(QFont("Arial", 14));

    painter->drawRect(boundingRect());

    if (alarmWindow.flashState == true) {
        button2.setVisible(true);
    } else {
        button2.setVisible(false);
    }

    if(isActive()) {
        update();
    }


}

void emsFull::setupRpmIndicator()
{
    double minValue, maxValue;
    minValue = gaugeSettings.value("RPM/min",0).toInt();
    maxValue = gaugeSettings.value("RPM/max",0).toInt();
    rpmIndicator.setPos(410, 140);
    rpmIndicator.setStartSpan(230.0, 240.0);
    rpmIndicator.setBorders(minValue, maxValue);

    for(int i=0;(i<=maxValue);i=i+1000) {
        rpmIndicator.addBetweenValue(i);
    }
    this->addItem(&rpmIndicator);
}

void emsFull::setupChtEgt()
{
    chtEgt.setPos(680, 450);
    this->addItem(&chtEgt);
}

void emsFull::setupBarGraphs()
{
    oilTemperature.setPos(620, 65);
    oilTemperature.setTitle("OIL T");
    oilTemperature.setUnit(settings.value("Units/temp").toString().toLatin1());
    oilTemperature.setIndicatorSide("left");
    oilTemperature.setGaugeType("OilTemp");
    oilTemperature.setSmoothBool(true);
    this->addItem(&oilTemperature);

    oilPressure.setPos(690, 65);
    oilPressure.setTitle("OIL P");
    oilPressure.setUnit(settings.value("Units/pressure").toString().toLatin1());
    oilPressure.setGaugeType("OilPress");
    this->addItem(&oilPressure);

    voltMeter.setPos(760, 65);
    voltMeter.setTitle("VOLTS");
    voltMeter.setUnit("V");
    voltMeter.setPrecision(1, 1);
    voltMeter.setIndicatorSide("left");
    voltMeter.setGaugeType("Volt");
    this->addItem(&voltMeter);

    ampereMeter.setPos(760, 205);
    ampereMeter.setTitle("LOAD");
    ampereMeter.setUnit("A");
    ampereMeter.addBetweenValue(0.0);
    ampereMeter.setGaugeType("Amp");
    this->addItem(&ampereMeter);

    fuelFlow.setPos(690, 205);
    fuelFlow.setTitle("FF");
    fuelFlow.setUnit(settings.value("Units/fuelFlow").toString().toLatin1());
    fuelFlow.setPrecision(1);
    fuelFlow.setIndicatorSide("left");
    fuelFlow.setGaugeType("FuelFlow");
    this->addItem(&fuelFlow);
    fuelFlow.setVisible(true);

    fuelPressure.setPos(620, 205);
    fuelPressure.setTitle("FUEL P");
    fuelPressure.setUnit(settings.value("Units/pressure").toString().toLatin1());
    fuelPressure.setPrecision(1);
    fuelPressure.setIndicatorSide("left");
    fuelPressure.setGaugeType("FuelPressure");
    this->addItem(&fuelPressure);
    fuelPressure.setVisible(true);

//    insideAirTemperature.setPos(800, 205);
//    insideAirTemperature.setTitle("IAT");
//    insideAirTemperature.setUnit(settings.value("Units/temp").toString().toLatin1());
//    insideAirTemperature.setPrecision(1);
//    this->addItem(&insideAirTemperature);
//    insideAirTemperature.setVisible(false);
//    connect(&outsideAirTemperature, SIGNAL(hasBeenClicked()), &outsideAirTemperature, SLOT(makeInvisible()));
//    connect(&outsideAirTemperature, SIGNAL(hasBeenClicked()), &insideAirTemperature, SLOT(makeVisible()));

//    outsideAirTemperature.setPos(150, 375);
//    outsideAirTemperature.setTitle("OAT");
//    outsideAirTemperature.setUnit(QString::fromUtf8("°F") /*settings.value("Units/temp").toString().toLatin1()*/);
//    outsideAirTemperature.setPrecision(1);
//    this->addItem(&outsideAirTemperature);
//    connect(&insideAirTemperature, SIGNAL(hasBeenClicked()), &insideAirTemperature, SLOT(makeInvisible()));
//    connect(&insideAirTemperature, SIGNAL(hasBeenClicked()), &outsideAirTemperature, SLOT(makeVisible()));
}

void emsFull::setupFuelManagement()
{
    fuelManagement.setPos(-495, -240);
    fuelManagement.setScale(1.8);
    fuelManagement.setVisible(false);
    connect(&fuelFlow, SIGNAL(hasBeenClicked()), &fuelManagement, SLOT(activateOverlay()));
    this->addItem(&fuelManagement);
    fuelDisplay.setPos(215,435);
    this->addItem(&fuelDisplay);
}

void emsFull::setupManifoldPressure()
{
    manifoldPressure.setPos(130, 150);
    manifoldPressure.setStartSpan(240.0, 240.0);
    manifoldPressure.setBorders(10.0, 35.0, 15.0, 35.0);
    manifoldPressure.addBetweenValue(10.0);
    manifoldPressure.addBetweenValue(15.0);
    manifoldPressure.addBetweenValue(20.0);
    manifoldPressure.addBetweenValue(25.0);
    manifoldPressure.addBetweenValue(30.0);
    manifoldPressure.addBetweenValue(35.0);
    this->addItem(&manifoldPressure);
}

void emsFull::setFuelData(double fuelFlowValue, double fuelAbsoluteValue)
{
    fuelFlow.setValue(fuelFlowValue);
    fuelManagement.setFuelFlow(fuelFlowValue);
    fuelDisplay.setFuelFlow(fuelFlowValue);
    fuelManagement.reduceFuelAmount(fuelAbsoluteValue);
}

void emsFull::setupWindVector() {
    windVector.setPos(50, 385);
    this->addItem(&windVector);
    windVector.setVisible(true);
}

void emsFull::connectSignals() {

    qDebug()<<"Connecting flashing alarm signals";
    // Connect signals for alarm flashing
    connect(&flashTimer, SIGNAL(timeout()), &alarmWindow, SLOT(changeFlashState()));
    connect(&flashTimer, SIGNAL(timeout()), &rpmIndicator, SLOT(changeFlashState()));
    connect(&flashTimer, SIGNAL(timeout()), &chtEgt, SLOT(changeFlashState()));
    connect(&flashTimer, SIGNAL(timeout()), &oilPressure, SLOT(changeFlashState()));
    connect(&flashTimer, SIGNAL(timeout()), &oilTemperature, SLOT(changeFlashState()));
    connect(&flashTimer, SIGNAL(timeout()), &voltMeter, SLOT(changeFlashState()));
    connect(&flashTimer, SIGNAL(timeout()), &ampereMeter, SLOT(changeFlashState()));

    qDebug()<<"Connecting RPM signals";
    //  Connect signal for alarm from rpm indicator
    connect(&rpmIndicator, SIGNAL(sendAlarm(QString,QColor,bool)), &alarmWindow, SLOT(onAlarm(QString,QColor,bool)));
    connect(&rpmIndicator, SIGNAL(cancelAlarm(QString)), &alarmWindow, SLOT(onRemoveAlarm(QString)));

    qDebug()<<"Connecting CHT/EGT Signals";
    //  Connect signal for alarm from CHT/EGT
    connect(&chtEgt, SIGNAL(sendAlarm(QString,QColor,bool)), &alarmWindow, SLOT(onAlarm(QString,QColor,bool)));
    connect(&chtEgt, SIGNAL(cancelAlarm(QString)), &alarmWindow, SLOT(onRemoveAlarm(QString)));

    //  Connect signal for alarm from Volt Meter
    connect(&voltMeter, SIGNAL(sendAlarm(QString,QColor,bool)), &alarmWindow, SLOT(onAlarm(QString,QColor,bool)));
    connect(&voltMeter, SIGNAL(cancelAlarm(QString)), &alarmWindow, SLOT(onRemoveAlarm(QString)));

    //  Connect signal for alarm from OILT
    connect(&oilTemperature, SIGNAL(sendAlarm(QString,QColor,bool)), &alarmWindow, SLOT(onAlarm(QString,QColor,bool)));
    connect(&oilTemperature, SIGNAL(cancelAlarm(QString)), &alarmWindow, SLOT(onRemoveAlarm(QString)));

    //  Connect signal for alarm from OILP
    connect(&oilPressure, SIGNAL(sendAlarm(QString,QColor,bool)), &alarmWindow, SLOT(onAlarm(QString,QColor,bool)));
    connect(&oilPressure, SIGNAL(cancelAlarm(QString)), &alarmWindow, SLOT(onRemoveAlarm(QString)));

    //  Connect signal for alarm from ampere meter
    connect(&ampereMeter, SIGNAL(sendAlarm(QString,QColor,bool)), &alarmWindow, SLOT(onAlarm(QString,QColor,bool)));
    connect(&ampereMeter, SIGNAL(cancelAlarm(QString)), &alarmWindow, SLOT(onRemoveAlarm(QString)));

    // Connect buttonBar to the alarm window for alarm acknowledgement
    connect(&button2, SIGNAL(clicked(bool)), &alarmWindow, SLOT(onAlarmAck()));
    connect(&alarmWindow, SIGNAL(stopAlarmFlash()), this, SLOT(onAckAlarm()));
    connect(&button1, SIGNAL(clicked(bool)), this, SLOT(onButton1Press()));

//    // Connect buttonBar to the fuelDisplay window to increment fuel amount
//    connect(&buttonBar, SIGNAL(sendFuelChange(QString)), &fuelDisplay, SLOT(onFuelAmountChange(QString)));

    // Connect signal for a flashing alarm to the button bar to be able to show the 'Ack' button
    connect(&alarmWindow, SIGNAL(flashingAlarm()), this, SLOT(onAlarmFlash()));

    // Connect signal to stop flashing alarm after it has been acknowledged
    connect(&alarmWindow, SIGNAL(stopAlarmFlash()), &chtEgt, SLOT(onAlarmAck()));
    connect(&alarmWindow, SIGNAL(stopAlarmFlash()), &voltMeter, SLOT(onAlarmAck()));
    connect(&alarmWindow, SIGNAL(stopAlarmFlash()), &oilTemperature, SLOT(onAlarmAck()));
    connect(&alarmWindow, SIGNAL(stopAlarmFlash()), &oilPressure, SLOT(onAlarmAck()));
    connect(&alarmWindow, SIGNAL(stopAlarmFlash()), &ampereMeter, SLOT(onAlarmAck()));
    connect(&alarmWindow, SIGNAL(stopAlarmFlash()), &rpmIndicator, SLOT(onAlarmAck()));

    qDebug()<<"Connecting hobb/flight time Signals";
    // Connect a timer for handling hobbs/flight time
    connect(&clockTimer, SIGNAL(timeout()), &hobbs, SLOT(onTic()));
    connect(&clockTimer, SIGNAL(timeout()), this, SLOT(setEngineConds()));
}

void emsFull::setupHourMeter() {
    hobbs.setPos(55, 360);
    this->addItem(&hobbs);
    hobbs.setVisible(true);
}

void emsFull::setEngineConds() {
    if (oilPressure.getValue() > 10.0) {
        hobbs.setEngineOn(true);
    } else {
        hobbs.setEngineOn(false);
    }

    if (oilTemperature.getValue() < warmupTemp) {
        timeOilTBelowWarmup += 1;
        if (timeOilTAboveWarmup < 5 * 60) {
            timeOilTAboveWarmup = 0;
            rpmIndicator.isWarmup = true;
        }
    } else {
        timeOilTAboveWarmup += 1;
        timeOilTBelowWarmup = 0;
        rpmIndicator.isWarmup = false;
    }
}

void emsFull::onEngineValuesUpdate(qreal rpm, qreal fuelFlow, qreal oilTemp, qreal oilPress, qreal amps, qreal volts, qreal egt1, qreal egt2, qreal egt3, qreal egt4, qreal cht1, qreal cht2, qreal cht3, qreal cht4, qreal oat, qreal iat, qreal map) {
    rpmIndicator.setValue(rpm);
    fuelDisplay.setFuelFlow(fuelFlow);
    //fuelFlow.setValue(fuelFlow);
    oilTemperature.setValue(oilTemp);
    oilPressure.setValue(oilPress);
    ampereMeter.setValue(amps);
    voltMeter.setValue(volts);
    chtEgt.setEgtValues(egt1, egt2, egt3, egt4);
    chtEgt.setChtValues(cht1, cht2, cht3, cht4);
    outsideAirTemperature.setValue(oat);
    insideAirTemperature.setValue(iat);
    manifoldPressure.setValue(map);

    if (oilTemp < warmupTemp ) {
        rpmIndicator.isWarmup = true;
    } else {
        rpmIndicator.isWarmup = false;
    }

    if (rpm > 0) {
        hobbs.setEngineOn(true);
    }
}

////
void emsFull::demoFunction()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
    static double rpm = 1100.0;
    rpm += 5.0;

    rpmIndicator.setValue(rpm);
    manifoldPressure.setValue(10);

    static double basicEGT = 750.0;
    static bool egtUp = true;
    static bool leaned = false;
    static double off13 = 0.0;
    static double off24 = 0.0;
    if(leaned)
    {
        if(basicEGT < 680.0)
        {
            off13 = double(qrand())/double(RAND_MAX)*3.0;
            off24 = double(qrand())/double(RAND_MAX)*5.0;
        }
    }
    else if(egtUp)
    {
        basicEGT += 1.0;
    }
    else
    {
        basicEGT -= 1.0;
    }
    if(basicEGT > 1200.0 && egtUp)
    {
        egtUp = false;
    }
    if(basicEGT < 1150.0 && !egtUp)
    {
        leaned = true;
        egtUp = true;
    }
    chtEgt.setEgtValues(basicEGT+51.0+off13, basicEGT+10.0-off24, basicEGT+5.0-off13, basicEGT+30.0+off24);

    static double basicCHT = 60.0;

    if(basicCHT > 250.0)
    {
        basicCHT -= 0.5;
    } else {
        basicCHT += 0.5;
    }
    static double offset1 = double(qrand())/double(RAND_MAX)*50.0;
    static double offset2 = double(qrand())/double(RAND_MAX)*7.0;
    static double offset3 = double(qrand())/double(RAND_MAX)*15.0;
    static double offset4 = double(qrand())/double(RAND_MAX)*9.0;
    chtEgt.setChtValues(basicCHT+offset1,
                        basicCHT-offset2,
                        basicCHT+offset3,
                        basicCHT-offset4);

    static double oilTemp = 100.0;
    if(oilTemp < 80.0)
    {
        oilTemp = 100.0;
    }
    oilTemp += 0.25;

    oilTemperature.setValue(oilTemp);

    static double oilPress = 0.0;
    oilPress += 0.05;
    if(oilPress > 60.0)
    {
        oilPress = 0.0;
    }
    oilPressure.setValue(oilPress);

    static double volts = 11.5;
    volts += 0.01;
    if(volts > 18.0)
    {
        volts = 11.5;
    }
    voltMeter.setValue(volts);

    static double amperes = 35.0;
    amperes -= 0.1;
    if(amperes < -20.0)
    {
        amperes = 30.0;
    }
    ampereMeter.setValue(amperes);

    static double flow = 7.0;
    flow -= 0.05;
    if(flow < 0.0)
    {
        flow = 7.0;
    }
    fuelFlow.setValue(flow);
    fuelManagement.setFuelFlow(flow);
    fuelManagement.reduceFuelAmount(flow*200.0/1000.0/60.0/60.0);
    fuelDisplay.setFuelFlow(flow);
    fuelDisplay.reduceFuelAmount(flow*200.0/1000.0/60.0/60.0);

    static double airTemp = -10.0;
    airTemp += 0.07;
    if(airTemp > 40.0)
    {
        airTemp = -10.0;
    }
    outsideAirTemperature.setValue(airTemp);
    insideAirTemperature.setValue(airTemp);

}
