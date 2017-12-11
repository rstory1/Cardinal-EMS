#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QThread>
#include <QTcpSocket>

//! [0]
class DataThread : public QThread
{
    Q_OBJECT

public:
    DataThread(int socketDescriptor, const QString &fortune, QObject *parent);

    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    QString text;
};
//! [0]

#endif // DATATHREAD_H

