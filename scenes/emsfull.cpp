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
    setupHourMeter();
    setupStatusItem();

    setSceneRect(0,0,800,480);

    setBackgroundBrush(Qt::black);

    connectSignals();

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

void emsFull::setupRpmIndicator()
{
    double minValue, maxValue;
    minValue = gaugeSettings.value("RPM/min",0).toInt();
    maxValue = gaugeSettings.value("RPM/max",0).toInt();
    rpmIndicator.setPos(400, 170);
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
    voltMeter.setSmoothBool(true);
    this->addItem(&voltMeter);

    ampereMeter.setPos(760, 205);
    ampereMeter.setTitle("LOAD");
    ampereMeter.setUnit("A");
    ampereMeter.addBetweenValue(0.0);
    ampereMeter.setGaugeType("Amp");
    ampereMeter.setSmoothBool(true);
    this->addItem(&ampereMeter);

    ampereMeter2.setPos(760, 205);
    ampereMeter2.setTitle("LOAD2");
    ampereMeter2.setUnit("A");
    ampereMeter2.addBetweenValue(0.0);
    ampereMeter2.setGaugeType("Amp");
    this->addItem(&ampereMeter2);
    ampereMeter2.setVisible(false);

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

    insideAirTemperature.setPos(125, 300);
    insideAirTemperature.setTitle("INT T");
    insideAirTemperature.setUnit(settings.value("Units/temp").toString().toLatin1());
    insideAirTemperature.setPrecision(1);
    this->addItem(&insideAirTemperature);
    insideAirTemperature.setVisible(true);
//    connect(&outsideAirTemperature, SIGNAL(hasBeenClicked()), &outsideAirTemperature, SLOT(makeInvisible()));
//    connect(&outsideAirTemperature, SIGNAL(hasBeenClicked()), &insideAirTemperature, SLOT(makeVisible()));

    tcSensor1.setPos(55, 150);
    tcSensor1.setTitle("TC1");
    tcSensor1.setUnit(settings.value("Units/temp").toString().toLatin1());
    tcSensor1.setPrecision(0);
    this->addItem(&tcSensor1);
    tcSensor1.setVisible(true);

    tcSensor2.setPos(55, 225);
    tcSensor2.setTitle("TC2");
    tcSensor2.setUnit(settings.value("Units/temp").toString().toLatin1());
    tcSensor2.setPrecision(0);
    this->addItem(&tcSensor2);
    tcSensor2.setVisible(true);

    tcSensor3.setPos(55, 300);
    tcSensor3.setTitle("TC3");
    tcSensor3.setUnit(settings.value("Units/temp").toString().toLatin1());
    tcSensor3.setPrecision(0);
    this->addItem(&tcSensor3);
    tcSensor3.setVisible(false);

    outsideAirTemperature.setPos(55, 300);
    outsideAirTemperature.setTitle("OAT");
    outsideAirTemperature.setUnit(QString::fromUtf8("°F") /*settings.value("Units/temp").toString().toLatin1()*/);
    outsideAirTemperature.setPrecision(0);
    outsideAirTemperature.setSmoothBool(true);
    this->addItem(&outsideAirTemperature);
    outsideAirTemperature.setVisible(true);
//    connect(&insideAirTemperature, SIGNAL(hasBeenClicked()), &insideAirTemperature, SLOT(makeInvisible()));
//    connect(&insideAirTemperature, SIGNAL(hasBeenClicked()), &outsideAirTemperature, SLOT(makeVisible()));
}

void emsFull::setupFuelManagement()

{
    fuelDisplay.setPos(215,435);
    this->addItem(&fuelDisplay);
}

void emsFull::setupManifoldPressure()
{
    manifoldPressure.setPos(100, 160);
    manifoldPressure.setStartSpan(240.0, 240.0);
    manifoldPressure.setBorders(10.0, 35.0, 15.0, 35.0);
    manifoldPressure.addBetweenValue(10.0);
    manifoldPressure.addBetweenValue(15.0);
    manifoldPressure.addBetweenValue(20.0);
    manifoldPressure.addBetweenValue(25.0);
    manifoldPressure.addBetweenValue(30.0);
    manifoldPressure.addBetweenValue(35.0);
    manifoldPressure.setVisible(false);
    this->addItem(&manifoldPressure);
}

void emsFull::setFuelData(qreal fuelFlowValue, qreal fuelFlowRawValue, qreal fuelAbsoluteValue)
{
    fuelFlow.setValue(fuelFlowValue, fuelFlowRawValue);
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

    // Connect buttonBar to the alarm window for alarm acknowledgement
    connect(&button2, SIGNAL(clicked(bool)), &alarmWindow, SLOT(onAlarmAck()));
    //connect(&alarmWindow, SIGNAL(stopAlarmFlash()), this, SLOT(onAckAlarm()));
    connect(&button1, SIGNAL(clicked(bool)), this, SLOT(onButton1Press()));

    qDebug()<<"Connecting hobb/flight time Signals";
    // Connect a timer for handling hobbs/flight time
    connect(&clockTimer, SIGNAL(timeout()), &hobbs, SLOT(onTic()));
    connect(&clockTimer, SIGNAL(timeout()), this, SLOT(setEngineConds()));
    connect(&clockTimer, SIGNAL(timeout()), this, SLOT(onTic()));
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

void emsFull::setupStatusItem()
{
    statusItem.setPos(120, 435);
    this->addItem(&statusItem);
    statusItem.setFont(QFont("Arial", 18, QFont::Bold));
    statusItem.setDefaultTextColor(Qt::white);
    statusItem.setVisible(false);
}

void emsFull::onEngineValuesUpdate(qreal rpm, qreal fuelF, qreal oilTemp, qreal oilPress, qreal amps, qreal amps2, qreal volts, qreal egt1, qreal egt2, qreal egt3, qreal egt4, qreal cht1, qreal cht2, qreal cht3, qreal cht4, qreal oat, qreal iat, qreal map, qreal fuelP) {
    //rpmIndicator.setValue(rpm);
    //fuelDisplay.setFuelFlow(fuelF);
    //fuelFlow.setValue(fuelF);
    //fuelPressure.setValue(fuelP);
    //oilTemperature.setValue(oilTemp);
    //oilPressure.setValue(oilPress);
    //ampereMeter.setValue(amps);
    //ampereMeter2.setValue(amps2);
    //voltMeter.setValue(volts);
    //chtEgt.setEgtValues(egt1, egt2, egt3, egt4);
    //chtEgt.setChtValues(cht1, cht2, cht3, cht4);
    //outsideAirTemperature.setValue(oat);
    //insideAirTemperature.setValue(iat);
    //manifoldPressure.setValue(map);

//    emsSerialString = QString::number(rpmIndicator.getValue()) + "," + QString::number(fuelFlow.getValue()) + "," + QString::number(oilTemperature.getValue()) + "," + QString::number(oilPress) + "," + QString::number(ampereMeter.getValue()) + "," +
//            QString::number(ampereMeter.getValue()) + "," + QString::number(voltMeter.getValue()) + "," + QString::number(fuelPressure.getValue()) + "," + QString::number(chtEgt.getCurrentChtValues().at(0)) + "," + QString::number(chtEgt.getCurrentChtValues().at(1))+ "," + QString::number(oat);
//    emsSerialStringByteArray = emsSerialString.toLocal8Bit();

//    emit sendSerialData(emsSerialStringByteArray);
}

void emsFull::demoFunction()
{
    qsrand(QDateTime::currentDateTime().toTime_t());
    static double rpm = 1100.0;
    rpm += 5.0;

    //rpmIndicator.setValue(rpm);
    //manifoldPressure.setValue(10);

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
    //chtEgt.setEgtValues(basicEGT+51.0+off13, basicEGT+10.0-off24, basicEGT+5.0-off13, basicEGT+30.0+off24);

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
//    chtEgt.setChtValues(basicCHT+offset1,
//                        basicCHT-offset2,
//                        basicCHT+offset3,
//                        basicCHT-offset4);

    static double oilTemp = 100.0;
    if(oilTemp < 80.0)
    {
        oilTemp = 100.0;
    }
    oilTemp += 0.25;

    //oilTemperature.setValue(oilTemp);

    static double oilPress = 0.0;
    oilPress += 0.05;
    if(oilPress > 60.0)
    {
        oilPress = 0.0;
    }
    //oilPressure.setValue(oilPress);

    static double volts = 11.5;
    volts += 0.01;
    if(volts > 18.0)
    {
        volts = 11.5;
    }
    //voltMeter.setValue(volts);

    static double amperes = 35.0;
    amperes -= 0.1;
    if(amperes < -20.0)
    {
        amperes = 30.0;
    }
    //ampereMeter.setValue(amperes);

    static double flow = 7.0;
    flow -= 0.05;
    if(flow < 0.0)
    {
        flow = 7.0;
    }
    //fuelFlow.setValue(flow);
    fuelManagement.setFuelFlow(flow);
    fuelManagement.reduceFuelAmount(flow*200.0/1000.0/60.0/60.0);
    fuelDisplay.setFuelFlow(flow);
    //fuelDisplay.reduceFuelAmount(flow*200.0/1000.0/60.0/60.0);

    static double airTemp = -10.0;
    airTemp += 0.07;
    if(airTemp > 40.0)
    {
        airTemp = -10.0;
    }
    //outsideAirTemperature.setValue(airTemp);
    //insideAirTemperature.setValue(airTemp);

}

void emsFull::onUpdateValues(qreal val0, qreal val1, qreal val2, qreal val3, qreal val4, qreal val5, qreal val6, qreal val7, qreal val8, qreal val9, qreal val10, qreal val11, qreal val12, qreal val13, qreal val14, qreal val15, qreal val16, qreal val17, qreal val18, qreal val19, qreal val20, qreal val21, qreal val22, qreal val23, qreal val24, qreal val25, qreal val26, qreal val27, qreal val28, QDateTime messageTime) {

    recordDateTime = messageTime;

    if (abs(messageTime.msecsTo(QDateTime::currentDateTime())) > 5000000000 /*17973121*/) {
        statusItem.setDefaultTextColor(Qt::red);
        statusItem.setPlainText("DATA NOT CURRENT");
        rpmIndicator.setValue(-999, -999);
        fuelDisplay.setFuelFlow(-999);
        fuelFlow.setValue(-999, -999);
        oilTemperature.setValue(-999, -999);
        oilPressure.setValue(-999, -999);
        ampereMeter.setValue(-999, -999);
        ampereMeter2.setValue(-999, -999);
        voltMeter.setValue(-999, -999);
        chtEgt.setEgtValues(-999, -999, -999, -999,-999, -999, -999, -999);
        chtEgt.setChtValues(-999, -999, -999, -999,-999, -999, -999, -999);
        outsideAirTemperature.setValue(-999, -999);
        insideAirTemperature.setValue(-999, -999);
        manifoldPressure.setValue(-999, -999);
        fuelPressure.setValue(-999, -999);
        tcSensor1.setValue(-999, -999);
        tcSensor2.setValue(-999, -999);
        tcSensor3.setValue(-999, -999);
    } else {
        statusItem.setDefaultTextColor(Qt::white);
        statusItem.setPlainText(""/*Values Updated at:" + recordDateTime.toString("MM-dd-yy hh:mm:ss.zzz")*/);
        rpmIndicator.setValue(val25, val12);
        fuelDisplay.setFuelFlow(val19);
        fuelFlow.setValue(val19, val6);
        oilTemperature.setValue(val24, val11);
        oilPressure.setValue(val23, val10);
        ampereMeter.setValue(val17, val4);
        ampereMeter2.setValue(val16, val3);
        voltMeter.setValue(val15, val2);
        chtEgt.setEgtValues(-999, -999, -999, -999,-999, -999, -999, -999);
        chtEgt.setChtValues(val22, val9, val21, val8, -999, -999, -999, -999);
        outsideAirTemperature.setValue(val14, val1);
        insideAirTemperature.setValue(val13, val0);
        manifoldPressure.setValue(val18, val5);
        fuelPressure.setValue(val20, val7);
        tcSensor1.setValue(val26, val26);
        tcSensor2.setValue(val27, val27);
        tcSensor3.setValue(val28, val28);
    }

    statusItem.update();

    emsSerialString = QString::number(val25) /* rpm */ + "," + QString::number(val19) /* Fuel Flow */ + "," + QString::number(oilTemperature.getValue()) /* oil temp */ + "," + QString::number(val23) /* oil pressure */ + "," + QString::number(val17) /* amps 1 */ + "," +
                QString::number(val16) /* amps 2 */ + "," + QString::number(val15) /* volts */ + "," + QString::number(val20) /* fuel pressure */ + "," + QString::number(val22) /* coolant 1 */ + "," + QString::number(val21) /* coolant 2 */ + "," + QString::number(val14) /* OAT */ + "," +
                QString::number(val18) /* manifold pressure */ + "," + QString::number(val13) /* internal rdac temp */ + "," + QString::number(val26) /* thermocouple 1 */ + "," + QString::number(val27) /* thermocouple 2 */ + "," + QString::number(val28) /* thermo couple 3 */ + "," +
                hobbs.getHobbsTime() + "," + hobbs.getFlightTime();

    emsSerialStringByteArray = emsSerialString.toLocal8Bit();

    emit sendSerialData(emsSerialStringByteArray);
}

emsFull::~emsFull() {

}

quint64 emsFull::elapsed() {
    quint64 elapsed = qAbs(recordDateTime.date().daysTo(QDateTime::currentDateTime().date()));
    elapsed *= static_cast<quint64>(24); // days to hours
    elapsed *= static_cast<quint64>(60); // hours to minutes
    elapsed *= static_cast<quint64>(60); // minutes to seconds
    elapsed *= static_cast<quint64>(1000); // seconds to milliseconds
    elapsed += qAbs(recordDateTime.time().msecsTo(QDateTime::currentDateTime().time()));
    return elapsed;
}

