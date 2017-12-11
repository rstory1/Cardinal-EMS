#include "dataserver.h"
#include "datathread.h"

#include <stdlib.h>

//! [0]
DataServer::DataServer(QObject *parent)
    : QTcpServer(parent)
{
    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");
}
//! [0]

//! [1]
void DataServer::incomingConnection(qintptr socketDescriptor)
{
    QString fortune = fortunes.at(qrand() % fortunes.size());
    DataThread *thread = new DataThread(socketDescriptor, dataString, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
//! [1]

void DataServer::onReceiveData(double rpmVal, double fuelFlowVal) {
    rpm = rpmVal;
    fuelFlow = fuelFlowVal;

    dataString = QString::number(rpm) + ";" + QString::number(fuelFlow);
}

