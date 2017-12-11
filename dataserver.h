#ifndef DATASERVER_H
#define DATASERVER_H

#include <QStringList>
#include <QTcpServer>

//! [0]
class DataServer : public QTcpServer
{
    Q_OBJECT

public:
    DataServer(QObject *parent = 0);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QStringList fortunes;
    double rpm;
    double fuelFlow;
    QString dataString;

signals:
    void requestData();

public slots:
    void onReceiveData(double rpmVal, double fuelFlowVal);
};
//! [0]

#endif  // DATASERVER_H
