#ifndef SPATIAL_H
#define SPATIAL_H

#include <QObject>
#include <sqlite3.h>
#include <QtSql>

class spatial : public QObject
{
    Q_OBJECT
public:
    explicit spatial(QObject *parent = 0);

private:
    int enable_spatialite(QSqlDatabase db);
signals:

public slots:
};

#endif // SPATIAL_H
