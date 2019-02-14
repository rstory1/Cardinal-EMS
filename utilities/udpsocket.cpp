//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CardinalEMS,   a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2017 Ryan Story                                        //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

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
