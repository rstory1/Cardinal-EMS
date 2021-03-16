#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QtCore>

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

signals:
    void updateSensorValues(qreal val0, qreal val1, qreal val2, qreal val3, qreal val4, qreal val5, qreal val6, qreal val7, qreal val8, qreal val9
                            ,qreal val10, qreal val11, qreal val12, qreal val13, qreal val14, qreal val15, qreal val16, qreal val17, qreal val18, qreal val19, QDateTime recordEnteredTime);

public slots:
    void executeSensorValueSqlQuery();

};

#endif // DATABASEHANDLER_H
