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

#include "sensorconvert.h"

SensorConvert::SensorConvert(QObject *parent) : QThread(parent)
  ,settings(QCoreApplication::applicationDirPath() + "/ems/settings/settings.ini", QSettings::IniFormat, parent)
  ,gaugeSettings(QCoreApplication::applicationDirPath() + "/ems/settings/gaugeSettings.ini", QSettings::IniFormat, parent)
{
    //Let's set what type of thermocouple we are using
    setThermocoupleTypeCht(settings.value("Sensors/chtThermocoupleType", "NTC").toString());
    setThermocoupleTypeEgt(settings.value("Sensors/egtThermocoupleType", "K").toString());
    setTemperatureScale(settings.value("Units/temp", "F").toString());
    setKFactor(gaugeSettings.value("FuelFlow/kfactor", "F").toString().toDouble());

//    qDebug() << temperatureScale;

    connect(&timerPulses, SIGNAL(timeout()), this, SLOT(debugSend()));
    timerPulses.start(10); //time specified in ms

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        qDebug() << "TRying to open DB";
        databaseLogging = QSqlDatabase::addDatabase(DRIVER, "sensorConvert");

        databaseLogging.setDatabaseName(QCoreApplication::applicationDirPath() + "/ems/engineLogs/emsData");

        if(!databaseLogging.open())
            qWarning() << "SensorConvert::SensorConvert - ERROR: " << databaseLogging.lastError().text();

        qDebug() << "DB SHould be open" << databaseLogging.isOpen();

        sqlQuery = QSqlQuery(databaseLogging);
    }
    else
        qWarning() << "SensorConvert::SensorConvert - ERROR: no driver " << DRIVER << " available";

}

void SensorConvert::convertOilTemp(double adc)
{
    //  Convert ADC Voltage value to resistance
    resistance = puResistorValue/(4095/adc-1);

    // Convert resistance to temperauter. This equation was created from fitting a line to the VDO calibration curve.
    temp = 429.7*pow(resistance, -0.1368)-143.9;

    // If our desired scale is not celsius, then we need to convert it
    if (temperatureScale != "C")
    {
        oilTemp = convertTemperature(temp, 'F');
    }
}

void SensorConvert::convertFuelFlow(qreal pulses)
{
    // User enters k factor which is used to convert pulses from the sensor into flow. The FT-60 lists its K Factor as 68E3 pulses/gal
    // The pulses are converted after being read to be in pulses/hour to standardize the equation and leave the sensor specific conversion to the data reading classes.
    if (pulses <= 0) {
        fuelFlow = 0.0;
    } else {
        // This gives fuel flow in gallons/hour
        fuelFlow = pulses / kFactor;
    }

//    if (popPulses) {
//        QMessageBox msgBox;
//        msgBox.setText("FF Pulses: " + QString::number(pulses));
//        msgBox.exec();

//        popPulses = false;
//    }

}

void SensorConvert::convertRpm(double pulses)
{
    // The Rotax sendor sends one pulse for every crankshaft revolution
    //  SO we just return the number of pulses until we hear something different
    rpm =  pulses;
}

void SensorConvert::convertOilPress(double adc)
{
    // 456-180 (Keller)
    tempVoltage = adc / (4095.0/5.0);
    tempCurrent = tempVoltage / pdResistorValue;
    oilPress =  625.0*tempCurrent - 2.5;

    // Conert to psi since equation produces reading in bars
    oilPress = oilPress * 14.5038;
}

void SensorConvert::convertOAT(qreal adc)
{

    //  Convert ADC Voltage value to resistance
    resistance = 10000.0/((4095.0/adc)-1.0);

    // Convert resistance to temperature of the Littelfuse NTC Thermistor
    oat = -20.14*log10(resistance)+211.7;
    oat = 0.0031*(oat*oat)+0.9713*oat-2.3687; // Celsius

    // If our desired scale is not Celsius, then we need to convert it
    if (temperatureScale != "C")
    {
        oat = convertTemperature(oat, 'F');
    }


}

void SensorConvert::convertIat(double sensorValue)
{
    iat = sensorValue;
}

double SensorConvert::convertTemperature(qreal temp, char convertToUnits)
{
    if (convertToUnits == 'F') {
        temp = temp * 1.8 + 32;
    } else {
        temp = (temp - 32) * 0.5555;
    }


    return temp;
}

void SensorConvert::setThermocoupleTypeCht(QString type)
{
    thermocoupleTypeCht = type;
}

void SensorConvert::setThermocoupleTypeEgt(QString type)
{
    thermocoupleTypeEgt = type;
}

void SensorConvert::setTemperatureScale(QString scale)
{
    temperatureScale = scale;
}

void SensorConvert::convertCht(qreal adc1, qreal adc2, qreal adc3, qreal adc4)
{
    tempCHT[0] = adc1;
    tempCHT[1] = adc2;
    tempCHT[2] = adc3;
    tempCHT[3] = adc4;

    //  Need to add switch in here for different CHT sensor types. The code below is for the Rotax NTC style
//    if (thermocoupleTypeCht=='NTC')
//    {
        for (int a = 0; a < 4; a++)
        {
            //  Convert ADC Voltage value to resistance
            resistance = puResistorValue/(4095.0/tempCHT[a]-1);

            // Convert resistance to temperauter. This equation was created from fitting a line to the VDO calibration curve.
            cht[a] = 429.7*pow(resistance, -0.1368)-143.9;

            // If our desired scale is not celsius, then we need to convert it
            if (temperatureScale != "C")
            {
                cht[a] = convertTemperature(cht[a], 'F');
            }
        }
//    }

}

//void SensorConvert::convertEgt(double volt1, double volt2, double volt3, double volt4)
//{

//}

void SensorConvert::onRdacUpdate(qreal fuelFlow1, qreal fuelFlow2, quint16 tc1, quint16 tc2, quint16 tc3, quint16 tc4, quint16 tc5, quint16 tc6, quint16 tc7, quint16 tc8, qreal oilT, qreal oilP, qreal ax1, qreal ax2, qreal fuelP, qreal coolantT, qreal fuelL1, qreal fuelL2, quint16 rpm1, qreal rpm2, qreal map, qreal curr, quint16 intTemp, qreal volts) {
    convertFuelFlow(fuelFlow1);
    convertOilPress(oilP);
    convertCht(ax1, ax2, tc3, tc4);
    convertOilTemp(oilT);
    convertCurrent(curr,1);
    convertCurrent(fuelL2,2);
    convertMAP(fuelL1); // Using fuelL1 since the MAP message from the RDAC is dependent on having the sensor integral to the RDAC
    convertOAT(coolantT);
    convertFuelP(fuelP);

    sqlStatement = "INSERT INTO rawSensorData ( raw_iat, raw_oat, raw_batteryVoltage, raw_current2, raw_current1, raw_manPressure, raw_fuelFlow, raw_fuelPressure, raw_cht2, raw_cht1, raw_oilPressure, \
                    raw_oilTemp, raw_rpm, iat, oat, batteryVoltage, current2, current1, manPressure, fuelFlow, fuelPressure, cht2, cht1, oilPressure, oilTemp, rpm, flightTime, hobbs, timeStamp) \
        VALUES (" + QString::number(intTemp) + "," + QString::number(coolantT) + "," + QString::number(volts) + "," + QString::number(fuelL2) + "," + QString::number(curr) \
                + "," + QString::number(fuelL1) + "," + QString::number(fuelFlow1) + "," + QString::number(fuelP) + "," + QString::number(ax2) + "," + QString::number(ax1) \
                + "," + QString::number(oilP) + "," + QString::number(oilT) + "," + QString::number(rpm1) + "," + QString::number(intTemp) + "," + QString::number(oat) \
                + "," + QString::number(volts) + "," + QString::number(current2) + "," + QString::number(current1) + "," + QString::number(manP) + "," + QString::number(fuelFlow) \
                + "," + QString::number(fuelPress) + "," + QString::number(cht[1]) + "," + QString::number(cht[0]) + "," + QString::number(oilPress) + "," + QString::number(oilTemp) \
                + "," + QString::number(rpm1) + ",'" + currentFlightTime+ "'," + QString::number(currentHobbs) + ",'" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss.zzz") + "');";

    databaseLogging.transaction();

    if(!sqlQuery.exec(sqlStatement))
        qWarning() << "SensorConvert::onRdacUpdate - ERROR: " << sqlQuery.lastError().text() << "; " << sqlStatement;

    databaseLogging.commit();

    emit updateMonitor(rpm1, fuelFlow, oilTemp, oilPress, current1, current2, volts, tc1, tc2, tc3, tc4, cht[0], cht[1], cht[2], cht[3], oat, intTemp, manP, fuelPress);
}

void SensorConvert::setKFactor(qreal kFac) {
    kFactor = kFac;
}

void SensorConvert::convertCurrent(qreal adc, int sensorNum)
{
//    currentAdc = adc;
//    current = 0.0244 * currentAdc - 50.024; // MGL Current Sensor
    if (sensorNum==1) {
        current1 = 73.3 * (adc / (4095/5)) / 5 - 36.7 + 0.53; // Pololu sensor acs711ex (+0.53 is correction factor for install.
    } else {
        current2 = 73.3 * (adc / (4095/5)) / 5 - 36.7 + 0.53; // Pololu sensor acs711ex (+0.53 is correction factor for install.
    }

}

void SensorConvert::onZeroCurrent() {
    gaugeSettings.setValue("Amps/zeroVal", currentAdc);
}

void SensorConvert::convertMAP(qreal adc) {
    manP = 39.958 * (adc / (4095/5)) + 13.26;

    manP = manP * 0.2953; // COnvert from kPa to in Hg
}

void SensorConvert::convertFuelP(qreal adc)
{
    // Kavlico P4055-15G
    fuelPress =  3.75*(adc / (4095.0/5.0)) - 1.875;
}

void SensorConvert::showPulses() {
    popPulses = true;
    timerPulses.start(30000);
}

qreal SensorConvert::filterReading(qreal inputVal, qreal preVal, qreal dt, qreal tau)
{
    return ((preVal - inputVal) * tau) / (tau + dt) + inputVal;
}

void SensorConvert::debugSend() {
    onRdacUpdate(4400, 1200, 0, 0, 0, 0, 0, 0, 0, 0, 4, 450, 4, 2.5, 2.5, 3.0, 2.3, 2.9, 2500, 0, 2.7, 2.8, 25, 14.2);
    //emit updateMonitor(5200, 5.0, 130, 65, 10, 10, 14.0, 0, 0, 0, 0, 100, 0, 0, 0, 20, 10, 8.6, 5.3);
}

void SensorConvert::onUpdateFlightTime(qreal hobbs, QString flightTime) {
    currentHobbs = hobbs;
    currentFlightTime = flightTime;
}
