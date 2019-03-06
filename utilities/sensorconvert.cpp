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
    setKFactor(gaugeSettings.value("Fuel/kfactor", "F").toString().toDouble());

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
        oilTemp = convertTemperature(temp);
    }
}

void SensorConvert::convertFuelFlow(qreal pulses)
{
    // User enters k factor which is pulse for one volumetric unit of fluid.
    // The eninge interface data will be coming in pulses per hour.
    fuelFlow = pulses / kFactor;
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
    tempVoltage = adc / (4095/5);
    tempCurrent = tempVoltage / pdResistorValue;
    oilPress =  0.625*tempCurrent + 0.75;
}

void SensorConvert::convertOat(double sensorValue)
{
    oat = sensorValue;
}

void SensorConvert::convertIat(double sensorValue)
{
    iat = sensorValue;
}

double SensorConvert::convertTemperature(qreal temp)
{
    // Code needs to be added to convert to different units
    temp = temp * 1.8 + 32;

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
                cht[a] = convertTemperature(cht[a]);
            }
        }
//    }

}

void SensorConvert::convertEgt(double volt1, double volt2, double volt3, double volt4)
{

}

void SensorConvert::onRdacUpdate(qreal fuelFlow1, qreal fuelFlow2, quint16 tc1, quint16 tc2, quint16 tc3, quint16 tc4, quint16 tc5, quint16 tc6, quint16 tc7, quint16 tc8, qreal oilT, qreal oilP, qreal ax1, qreal ax2, qreal fuelP, qreal coolantT, qreal fuelL1, qreal fuelL2, quint16 rpm1, qreal rpm2, qreal map, qreal curr, quint16 intTemp, qreal volts) {
    convertFuelFlow(fuelFlow1);
    convertOilPress(oilP);
    convertCht(ax1, ax2, tc3, tc4);
    convertOilTemp(oilT);
    convertCurrent(curr);
    convertMAP(map);

    emit updateMonitor(rpm1, fuelFlow1, oilTemp, oilPress, current, volts, tc1, tc2, tc3, tc4, cht[0], cht[1], cht[2], cht[3], oat, intTemp, manP);
}

void SensorConvert::setKFactor(qreal kFac) {
    kFactor = kFac;
}

void SensorConvert::convertCurrent(qreal adc)
{
    currentAdc = adc;
    current = 0.0244 * currentAdc - 50.024;
}

void SensorConvert::onZeroCurrent() {
    gaugeSettings.setValue("Amps/zeroVal", currentAdc);
}

void SensorConvert::convertMAP(qreal adc) {
    manP = 5.7993 * (adc / (4095/5)) + 1.1599;
}
