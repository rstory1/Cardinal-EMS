#include "databasehandler.h"

DatabaseHandler::DatabaseHandler(QObject *parent) : QThread(parent)
{
    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        //qDebug() << "TRying to open DB";
        databaseLogging = QSqlDatabase::addDatabase(DRIVER, "sensorConvert");

        databaseLogging.setDatabaseName(QCoreApplication::applicationDirPath() + "/ems/engineLogs/emsData");

        if(!databaseLogging.open())
            qWarning() << "SensorConvert::SensorConvert - ERROR: " << databaseLogging.lastError().text();

        qDebug() << "DB Status: " << databaseLogging.isOpen();

        connect(&sensorReadTimer, SIGNAL(timeout()), this, SLOT(executeSensorValueSqlQuery()));

        sensorReadTimer.start(250);
    }
    else
        qWarning() << "SensorConvert::SensorConvert - ERROR: no driver " << DRIVER << " available";
}

void DatabaseHandler::executeSensorValueSqlQuery() {
//    QElapsedTimer timer;
//    QElapsedTimer timer2;
//    timer.start();

    QSqlQuery sqlQuery = QSqlQuery(databaseLogging);

    sqlStatement = "SELECT rpm, oilTemp, oilPressure, cht1, cht2, fuelPressure, fuelFLow, manPressure, current1, current2, batteryVoltage, oat, iat, hobbs, flightTime, timeStamp FROM rawSensorData ORDER BY dataIndex DESC LIMIT 1;";

    bool queryResult = sqlQuery.exec(sqlStatement);

    //qDebug() << queryResult;

    if(!queryResult)
        qWarning() << "emsFull::onEngineValuesUpdate - ERROR: " << sqlQuery.lastError().text() << "; " << sqlStatement;

    //qDebug() << sqlQuery.lastQuery();
    //qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";
//    timer2.start();
    sqlQuery.next();

    //qDebug() << "DB DateTime: " << sqlQuery.value(15).toDateTime() << "; Current DateTime: " << dateTime.currentDateTime();

    QList<qreal> dbValues = {sqlQuery.value(0).toReal(), sqlQuery.value(6).toReal(),sqlQuery.value(6).toReal(), sqlQuery.value(1).toReal(), sqlQuery.value(2).toReal()/
                      sqlQuery.value(8).toReal(), sqlQuery.value(9).toReal(), sqlQuery.value(10).toReal(), -999, -999/
                      -999, -999, sqlQuery.value(3).toReal(), sqlQuery.value(4).toReal(), -999, -999, sqlQuery.value(11).toReal(), sqlQuery.value(12).toReal()/
                      sqlQuery.value(7).toReal(), sqlQuery.value(5).toReal()};

    emit updateSensorValues(dbValues[0], dbValues[1], dbValues[2], dbValues[3], dbValues[4], dbValues[5], dbValues[6], dbValues[7], dbValues[8], dbValues[9], dbValues[10], dbValues[11], dbValues[12], dbValues[13], dbValues[14], dbValues[15], dbValues[16], dbValues[17], dbValues[18], dbValues[19], sqlQuery.value(15).toDateTime());
    //qDebug() << "2: The slow operation took" << timer2.elapsed() << "milliseconds";
}

void DatabaseHandler::executeInsertSensorValues(qreal intTempRaw, qreal coolantT, qreal voltsRaw, qreal fuelL2, qreal curr, qreal fuelL1, qreal fuelFlow1, qreal fuelP, qreal ax2, qreal ax1, qreal oilP, qreal oilT, qreal rpm1Raw, qreal intTemp, qreal oat, qreal volts, qreal current2, qreal current1, qreal manP, qreal fuelFlow, qreal fuelPress, qreal cht2, qreal cht1, qreal oilPress, qreal oilTemp, qreal rpm1) {
//    QElapsedTimer timer;
//    QElapsedTimer timer2;
//    timer.start();

    QSqlQuery sqlQuery = QSqlQuery(databaseLogging);

    sqlStatement = "INSERT INTO rawSensorData ( raw_iat, raw_oat, raw_batteryVoltage, raw_current2, raw_current1, raw_manPressure, raw_fuelFlow, raw_fuelPressure, raw_cht2, raw_cht1, raw_oilPressure, \
                    raw_oilTemp, raw_rpm, iat, oat, batteryVoltage, current2, current1, manPressure, fuelFlow, fuelPressure, cht2, cht1, oilPressure, oilTemp, rpm, flightTime, hobbs, timeStamp) \
        VALUES (" + QString::number(intTemp) + "," + QString::number(coolantT) + "," + QString::number(volts) + "," + QString::number(fuelL2) + "," + QString::number(curr) \
                + "," + QString::number(fuelL1) + "," + QString::number(fuelFlow1) + "," + QString::number(fuelP) + "," + QString::number(ax2) + "," + QString::number(ax1) \
                + "," + QString::number(oilP) + "," + QString::number(oilT) + "," + QString::number(rpm1) + "," + QString::number(intTemp) + "," + QString::number(oat) \
                + "," + QString::number(volts) + "," + QString::number(current2) + "," + QString::number(current1) + "," + QString::number(manP) + "," + QString::number(fuelFlow) \
                + "," + QString::number(fuelPress) + "," + QString::number(cht2) + "," + QString::number(cht1) + "," + QString::number(oilPress) + "," + QString::number(oilTemp) \
                + "," + QString::number(rpm1) + ",'" + currentFlightTime+ "'," + QString::number(currentHobbs) + ",'" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss.zzz") + "');";

    databaseLogging.transaction();

    if(!sqlQuery.exec(sqlStatement))
        qWarning() << "DatabaseHandler::executeInsertSensorValues - ERROR: " << sqlQuery.lastError().text() << "; " << sqlStatement;

    databaseLogging.commit();

    bool queryResult = sqlQuery.exec(sqlStatement);

    //qDebug() << queryResult;

    if(!queryResult)
        qWarning() << "emsFull::onEngineValuesUpdate - ERROR: " << sqlQuery.lastError().text() << "; " << sqlStatement;

    //qDebug() << sqlQuery.lastQuery();
    //qDebug() << "The slow operation took" << timer.elapsed() << "milliseconds";
//    timer2.start();

    //qDebug() << "2: The slow operation took" << timer2.elapsed() << "milliseconds";
}
