//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2012 Tobias Rad                                        //
//                2017 Ryan Story                                       //
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

#include "rdacconnect.h"

RDACXFmessage::RDACXFmessage() : flow1(0),
     pulseRatio1(0),
     flow2(0),
     pulseRatio2(0),
//     thermocouple(quint16(12)),
     oilTemp(0),
     oilPress(0),
     aux1(0),
     aux2(0),
     fuelPress(0),
     coolant(0),
     fuelLevel1(0),
     fuelLevel2(0),
     rpm1(0),
     rpm2(0),
     map(0),
     current(0),
     internalTemp(0),
     volts(0)
{
}

RDACconnect::RDACconnect(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
                SLOT(handleError(QSerialPort::SerialPortError)));


    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    openSerialPort();
}

void RDACconnect::readData()
{
    bool startPatternFound = false;

    data.append(serial->readAll());

//            emit userMessage("RDAC COM error", "Error reading data, closing application", true);
//            exec();
//        }

    if(searchStart(&data))
    {
        startPatternFound = true;
        quint8 messageType = 0x00;
        switch(checkPatternValidity(&data, messageType))
        {
        case rdacResultMessageComplete:
            //  Move the data to a buffer so we can process it while letting the next message get checked.

            qDebug() << "Everything OK - Last update: " << lastMessageReception.toString("hh:mm:ss.zzz");

            switch(messageType)
            {
                case 0x01:
                    handleMessageRDACXF(&data);
                    break;
                default:
                    data.remove(0, 1);
                    break;
            }
            break;
        case rdacResultMessageIncomplete:
            break;
        default:
            qDebug() << "Found pattern not valid";
            break;
        }
    }
    if(!startPatternFound)
    {
        qDebug() << "No start pattern found yet";
    }
}

quint8 RDACconnect::calculateChecksum1(QByteArray data)
{
    quint8 checksum = 0x55;
    //qDebug() << checksum;
    for(int i = 2; i < data.size()-2; ++i)
    {
        checksum += quint8(data.at(i));
    }
    return checksum;
}

quint8 RDACconnect::calculateChecksum2(QByteArray data)
{
    quint8 checksum = 0xAA;
    for(int i = 2; i < data.size()-2; ++i)
    {
        checksum += quint8(data.at(i));
    }
    return checksum;
}

bool RDACconnect::searchStart(QByteArray *data)
{
    while(data->size() >= 3)
    {
        if(data->at(0) == 0x05)
        {
            if(data->at(1) == 0x02)
            {
                if((data->at(2) == 0x01))
                {
                    numTries += 1;
                    return true;
                }
            }
        }
        data->remove(0, 1);
    }
    return false;
}

RDACconnect::rdacResults RDACconnect::checkPatternValidity(QByteArray *data, quint8 &messageType)
{
    // Determine and check neccessary size of data
    quint8 requiredSize = 0;
    messageType = data->at(2);
    switch(messageType)
    {
        case 0x01:
            requiredSize = 66;
            break;
        case 0x02:
            requiredSize = 23;
            break;
        case 0x03:
            requiredSize = 7;
            break;
        case 0x04:
            requiredSize = 29;
            break;
        default:
            data->remove(0, 1);
            return rdacResultMessageIllegalDatatype;
    }
    if(data->size() < requiredSize)
    {
        //qDebug() << "Message incomplete";
        return rdacResultMessageIncomplete;
    }

    // Calculate and check checksums
    if(quint8(data->at(requiredSize - 2)) == calculateChecksum1(data->mid(0, requiredSize)))
    {
        if(quint8(data->at(requiredSize - 1)) == calculateChecksum2(data->mid(0, requiredSize)))
        {
            //numSuccess += 1;
            //qDebug() << "Sucesses:" << numSuccess << "; Tries:" << numTries << "; " << numSuccess/numTries << "%";
            return rdacResultMessageComplete;
        }
        else
        {
            qWarning() << "Checksum 2 incorrect" << quint8(data->at(requiredSize - 1));
            data->remove(0, 1);
            return rdacResultMessageInvalidChecksum2;
        }
    }
    else
    {
        qWarning() << "Checksum 1 incorrect" << quint8(data->at(requiredSize - 2));
        data->remove(0, 1);
        return rdacResultMessageInvalidChecksum1;
    }
}

void RDACconnect::handleMessageRDACXF(QByteArray *data)
{
    lastMessageReception = QDateTime::currentDateTimeUtc();
//    QFile file(QCoreApplication::applicationDirPath() + "/ems/appLogs/datapacket.log");
//    file.open(QIODevice::WriteOnly | QIODevice::Append);
//    file.write(data->toHex());
//    file.close();
    RDACXFmessage message;
    memcpy(&message, data->constData(), 66);

    data->remove(0,66);

    if (message.pulseRatio1 == 65535) {
        message.pulseRatio1 = 0;
    }

    if (message.pulseRatio2 == 65535) {
        message.pulseRatio2 = 0;
    }

    //qDebug() << Q_FUNC_INFO << "Pulses" << message.flow1;

    volts = round(message.volts/5.73758)*0.1;

    lastMessage1 = QDateTime::currentDateTime();

    fuelFlow1 = (message.flow1 / 4) * 60.0 * 60.0; // This converts the pulse data from the RDAC (# of pulses per 4 second period) into pulses/hour
    fuelFlow2 = (message.flow2 / 4) * 60.0 * 60.0; // This converts the pulse data from the RDAC (# of pulses per 4 second period) into pulses/hour

    qDebug() << "RDAC Message Update Sent " + lastMessage1.toString("hh:mm:ss.zzz");

    sensorConvert.onRdacUpdate(fuelFlow1, fuelFlow2, message.thermocouple1 + message.internalTemp, message.thermocouple2 + message.internalTemp, message.thermocouple3, message.thermocouple4, message.thermocouple5, message.thermocouple6, message.thermocouple7, message.thermocouple8, message.oilTemp, message.oilPress, message.aux1, message.aux2, message.fuelPress, message.coolant, message.fuelLevel1, message.fuelLevel1, message.rpm1, message.rpm2, message.map, message.current, message.internalTemp, volts, lastMessage1);
}

void RDACconnect::openSerialPort()
{
    QSerialPortInfo portToUse;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString s = QObject::tr("Port:") + info.portName() + "\n"
                    + QObject::tr("Location:") + info.systemLocation() + "\n"
                    + QObject::tr("Description:") + info.description() + "\n"
                    + QObject::tr("Manufacturer:") + info.manufacturer() + "\n"
                    + QObject::tr("Serial number:") + info.serialNumber() + "\n"
                    + QObject::tr("Vendor Identifier:") + (info.hasVendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Product Identifier:") + (info.hasProductIdentifier() ? QString::number(info.productIdentifier(), 16) : QString()) + "\n"
                    + QObject::tr("Busy:") + (info.isBusy() ? QObject::tr("Yes") : QObject::tr("No")) + "\n";

        if(!info.isBusy() && (info.description().contains("Arduino") || info.manufacturer().contains("Arduino")))
            portToUse = info;
        qDebug() << s;
    }

//    if(portToUse.isNull() || !portToUse.isValid())
//    {
//        qDebug() << "port is not valid:" << portToUse.portName();
//        QMessageBox msgBox;
//        msgBox.setText("Port is invalid");
//        msgBox.exec();
//        return;
//    }

    // Enumerate the serial port
    // Open it if it isn't busy
    serial->setPortName(QString("/dev/ttyO4"));
    serial->setBaudRate(QSerialPort::Baud38400);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Connected to /dev/ttyO4";
    } else {
        qCritical() << "Serial Port error:" << serial->errorString();

        qDebug() << tr("Open error");
    }
}

void RDACconnect::closeSerialPort()
{
    serial->close();
    qDebug() << tr("Disconnected");
}

void RDACconnect::writeData(const QByteArray &data)
{
    serial->write(data);

    qDebug() << "Writing data out";
    qDebug() << data;
}

void RDACconnect::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qCritical() << "Serial Port error:" << serial->errorString();
        closeSerialPort();
    }
}
