#include "sensorconvert.h"

SensorConvert::SensorConvert(QObject *parent) : QThread(parent)
  ,settings("./settings.ini", QSettings::IniFormat, parent)
{
    //Let's set what type of thermocouple we are using
    setThermocoupleTypeCht(settings.value("Sensors/chtThermocoupleType", "K").toString());
    setThermocoupleTypeEgt(settings.value("Sensors/egtThermocoupleType", "K").toString());
    setTemperatureScale(settings.value("Units/temp", "F").toString());

}

void SensorConvert::convertOilTemp(double resistance)
{
    double temp;

    // Function created from data used by Aviasport Oil Temp instrument
    //  This equation produces a temperature in Celsius
    //  Rotax 912ULS
    if (resistance <= 322.8 && resistance >= 18.6) {
        temp = 497.2 * pow(resistance, -0.397);
    }
    else {
        temp = -999;
    }

    // If our desired scale is not celsius, then we need to convert it
    if (temperatureScale != "C")
    {
        temp = convertTemperature(temp);
    }

    oilTemp = temp;

}

void SensorConvert::convertFuelFlow(double pulses)
{
    // User enters k factor which is pulse for one volumetric unit of fluid
    fuelFlow = pulses / kFactor;
}

void SensorConvert::convertRpm(double pulses)
{
    // The Rotax sendor sends one pulse for every crankshaft revolution
    //  SO we just return the number of pulses until we hear something different
    rpm =  pulses;
}

void SensorConvert::convertOilPress(double voltage)
{
    // 456-180 (Keller)
    oilPress =  0.625*voltage + 0.75;
}

void SensorConvert::convertOat(double sensorValue)
{
    oat = sensorValue;
}

void SensorConvert::convertIat(double sensorValue)
{
    iat = sensorValue;
}

double SensorConvert::convertTemperature(double temp)
{
    // COde needs to be added to convert to different units
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

void SensorConvert::convertCht(double volt1, double volt2, double volt3, double volt4)
{

}

void SensorConvert::convertEgt(double volt1, double volt2, double volt3, double volt4)
{

}

void SensorConvert::processData(QString data)
{
    // Process the data string from the serial read. In the beginning, we're receiving one string with all the data.
    // 0 - RPM
    // 1 - Fuel Flow
    // 2 - Oil Temp
    // 3 - Oil Pressure
    // 4 - Amperage
    // 5 - Volts
    // 6-9 - EGT
    // 10-13 - CHT
    // 14 - OAT
    // 15 - IAT

    convertRpm(data.section(',',0,0).toDouble());
    convertFuelFlow(data.section(',',1,1).toDouble());
    convertOilTemp(data.section(',',2,2).toDouble());
    convertOilPress(data.section(',',3,3).toDouble());
    //convertAmperage(data.section(',',4,4).toDouble());
    //convertVolts(data.section(',',5,5).toDouble());
    convertEgt(data.section(',',6,6).toDouble(),data.section(',',7,7).toDouble(),data.section(',',8,8).toDouble(),data.section(',',9,9).toDouble());
    convertCht(data.section(',',10,10).toDouble(),data.section(',',11,11).toDouble(),data.section(',',12,12).toDouble(),data.section(',',13,13).toDouble());
    convertOat(data.section(',',14,14).toDouble());
    convertIat(data.section(',',15,15).toDouble());

    emit updateMonitor(rpm, fuelFlow, oilTemp, oilPress, amps, volts, egt1, egt2, egt3, egt4, cht1, cht2, cht3, cht4, oat, iat);
}

