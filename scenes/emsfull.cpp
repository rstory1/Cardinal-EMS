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
    //setupFuelManagement();
    //setupWindVector();
    setupHourMeter();

    setSceneRect(0,0,800,480);

    setBackgroundBrush(Qt::black);

    connectSignals();

    // Initialize the timer to flash values on alarm
    flashTimer.start(1000);

    clockTimer.start(1000);

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

    if(isActive()) {
        update();
    }


}

void emsFull::setupRpmIndicator()
{
    double minValue, maxValue;
    minValue = gaugeSettings.value("RPM/min",0).toInt();
    maxValue = gaugeSettings.value("RPM/max",0).toInt();
    rpmIndicator.setPos(450, 140);
    rpmIndicator.setStartSpan(230.0, 240.0);
    rpmIndicator.setBorders(minValue, maxValue);

    for(int i=0;(i<=maxValue);i=i+1000) {
        rpmIndicator.addBetweenValue(i);
    }
    this->addItem(&rpmIndicator);
}

void emsFull::setupChtEgt()
{
    chtEgt.setPos(700, 450);
    this->addItem(&chtEgt);
}

void emsFull::setupBarGraphs()
{
    oilTemperature.setPos(620, 65);
    oilTemperature.setTitle("OIL T");
    oilTemperature.setUnit(settings.value("Units/temp").toString().toLatin1());
    oilTemperature.setIndicatorSide("left");
    oilTemperature.setGaugeType("OilTemp");
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
    ampereMeter.setTitle("AMPS");
    ampereMeter.setUnit("A");
    ampereMeter.addBetweenValue(0.0);
    ampereMeter.setGaugeType("Amp");
    this->addItem(&ampereMeter);

    fuelFlow.setPos(760, 205);
    fuelFlow.setTitle("FF");
    fuelFlow.setUnit(settings.value("Units/fuelFlow").toString().toLatin1());
    fuelFlow.setPrecision(1);
    fuelFlow.setIndicatorSide("left");
    fuelFlow.setGaugeType("Fuel");
    this->addItem(&fuelFlow);
    fuelFlow.setVisible(false);

    insideAirTemperature.setPos(800, 205);
    insideAirTemperature.setTitle("IAT");
    insideAirTemperature.setUnit(settings.value("Units/temp").toString().toLatin1());
    insideAirTemperature.setPrecision(1);
    this->addItem(&insideAirTemperature);
    insideAirTemperature.setVisible(false);
    connect(&outsideAirTemperature, SIGNAL(hasBeenClicked()), &outsideAirTemperature, SLOT(makeInvisible()));
    connect(&outsideAirTemperature, SIGNAL(hasBeenClicked()), &insideAirTemperature, SLOT(makeVisible()));

    outsideAirTemperature.setPos(150, 375);
    outsideAirTemperature.setTitle("OAT");
    outsideAirTemperature.setUnit(QString::fromUtf8("°F") /*settings.value("Units/temp").toString().toLatin1()*/);
    outsideAirTemperature.setPrecision(1);
    this->addItem(&outsideAirTemperature);
    connect(&insideAirTemperature, SIGNAL(hasBeenClicked()), &insideAirTemperature, SLOT(makeInvisible()));
    connect(&insideAirTemperature, SIGNAL(hasBeenClicked()), &outsideAirTemperature, SLOT(makeVisible()));
}

void emsFull::setupFuelManagement()
{
    fuelManagement.setPos(-495, -240);
    fuelManagement.setScale(1.8);
    fuelManagement.setVisible(false);
    connect(&fuelFlow, SIGNAL(hasBeenClicked()), &fuelManagement, SLOT(activateOverlay()));
    this->addItem(&fuelManagement);
    fuelDisplay.setPos(102,102);
    this->addItem(&fuelDisplay);
}

void emsFull::setupManifoldPressure()
{
    manifoldPressure.setPos(150, 150);
    manifoldPressure.setStartSpan(240.0, 240.0);
    manifoldPressure.setBorders(10.0, 30.0, 13.0, 30.0);
    manifoldPressure.addBetweenValue(10.0);
    manifoldPressure.addBetweenValue(15.0);
    manifoldPressure.addBetweenValue(20.0);
    manifoldPressure.addBetweenValue(25.0);
    manifoldPressure.addBetweenValue(30.0);
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
    connect(this, SIGNAL(ackAlarm()), &alarmWindow, SLOT(onAlarmAck()));

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
