#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include <QUdpSocket>

class UdpSocket : public QObject
{
    Q_OBJECT
public:
    explicit UdpSocket(QObject *parent = 0);

    void WriteData();

signals:

public slots:
    void processPendingDatagrams();

private:
    QUdpSocket *socket;
};

#endif // UDPSOCKET_H
