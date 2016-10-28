#include "PortListener.h"
#include <QtDebug>

PortListener::PortListener(const QString &portName)
{
    qDebug() << "hi there";
    this->port = new QextSerialPort(portName, QextSerialPort::EventDriven);
    port->setBaudRate(BAUD9600);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

    if (port->open(QIODevice::ReadWrite) == true) {
        connect(port, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(port, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
        if (!(port->lineStatus() & LS_DSR))
            qDebug() << "warning: device is not turned on";
        qDebug() << "listening for data on" << port->portName();
    }
    else {
        qDebug() << "device failed to open:" << port->errorString();
    }
}

void PortListener::onReadyRead()
{
    QByteArray bytes;
    int a = port->bytesAvailable();
    bytes.resize(a);
    port->read(bytes.data(), bytes.size());

    bytesReceived.append(bytes);

    if(bytes.contains('\n')) {
        qDebug() << QString::fromLatin1(bytesReceived);
        QString data = QString::fromLatin1(bytesReceived);
        bytesReceived.clear();
        emit updateOilTemp(data.section(',',9,9).toDouble());
        emit updateEgtCht(data.section(',',1,1).toDouble(),data.section(',',2,2).toDouble(),data.section(',',3,3).toDouble(),data.section(',',4,4).toDouble(),data.section(',',5,5).toDouble(),data.section(',',6,6).toDouble(),data.section(',',7,7).toDouble(),data.section(',',8,8).toDouble());
        emit updateRpm(data.section(',',0,0).toDouble());
    }
}

void PortListener::onDsrChanged(bool status)
{
    if (status)
        qDebug() << "device was turned on";
    else
        qDebug() << "device was turned off";
}
