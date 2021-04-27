#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtCore>

#include <include/rdacconnect.h>
#include <include/sensorconvert.h>

class DatabaseHandler : public QThread
{
    Q_OBJECT
public:
    explicit DatabaseHandler(QObject *parent = nullptr);

private:
    QSqlDatabase databaseLogging;
    QString sqlStatement;
    const QString DRIVER = "QSQLITE";

    QTimer sensorReadTimer;

    QDateTime dateTime;

    double currentHobbs;
    QString currentFlightTime;

    qreal fuelLevel;

signals:
    void updateSensorValues(qreal val0, qreal val1, qreal val2, qreal val3, qreal val4, qreal val5, qreal val6, qreal val7, qreal val8, qreal val9
                            ,qreal val10, qreal val11, qreal val12, qreal val13, qreal val14, qreal val15, qreal val16, qreal val17, qreal val18, QDateTime recordEnteredTime);
    void sendFuelLevel(qreal level);

public slots:
    void executeSensorValueSqlQuery();
    void executeInsertSensorValues(qreal intTempRaw, qreal coolantT, qreal voltsRaw, qreal fuelL2, qreal curr, qreal fuelL1, qreal fuelFlow1, qreal fuelP, qreal ax2, qreal ax1, qreal oilP, qreal oilT, qreal rpm1Raw, qreal intTemp, qreal oat, qreal volts, qreal current2, qreal current1, qreal manP, qreal fuelFlow, qreal fuelPress, qreal cht2, qreal cht1, qreal oilPress, qreal oilTemp, qreal rpm1);
    void onReceiveTimeData(double hobbs, QString flightTime) {
        currentFlightTime = flightTime;
        currentHobbs = hobbs;
    }
    void onReceiveFuelLevel(qreal level);

    void onInitializeFuelLevel();
};

#endif // DATABASEHANDLER_H
