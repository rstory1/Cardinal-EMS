#include "udpsocket.h"

UdpSocket::UdpSocket(QObject *parent) : QObject(parent)
{
    //socket = new QUdpSocket(this);

    //socket->bind(49901);

    //connect(socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));

    //qDebug()<<"Creating";
}

void UdpSocket::WriteData()
{
    QByteArray msg;

    msg.append("Hello!!!");

    //socket->writeDatagram(msg,QHostAddress::LocalHost,5678);
}


void UdpSocket::processPendingDatagrams() {
    QByteArray datagram;
    //datagram.resize(socket->pendingDatagramSize());
    //socket->readDatagram(datagram.data(), datagram.size());

    //qDebug()<<"Processing";
    //qDebug()<<"Message: " << tr("Received datagram: \"%1\"").arg(datagram.data());
}
