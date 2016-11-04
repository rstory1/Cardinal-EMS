#include "sensorconvert.h"

SensorConvert::sensorConvert(QObject *parent) : QObject(parent)
{
    //Let's set what type of thermocouple we are using
    setThermocoupleTypeCht('K');
    setThermocoupleTypeEgt('K');
    setTemperatureScale('F');

}

double SensorConvert::convertOilTemp(int resistance)
{
    quint16 temp;

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

    return temp.toDouble();

}

double SensorConvert::convertFuelFlow(quint16 pulses)
{
    // User enters k factor which is pulse for one volumetric unit of fluid
    return pulses / kFactor;
}

double SensorConvert::convertRpm(quint16 pulses)
{
    // The Rotax sendor sends one pulse for every crankshaft revolution
    //  SO we just return the number of pulses until we hear something different
    return pulses.toDouble();
}

double SensorConvert::convertOilPress(int voltage)
{
    // 456-180 (Keller)
    return 0.625*voltage + 0.75;
}
