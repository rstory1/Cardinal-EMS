#ifndef ENGINEDATA_H
#define ENGINEDATA_H

#include <QtCore>

//! EngineData - POD struct carrying all converted sensor values
/*!
 * Replaces the 29-parameter updateValues signal signature with a single
 * struct, making the interface maintainable and self-documenting.
 * Field names match the original positional comments in sensorconvert.cpp.
 */
struct EngineData {
    qreal intTempRaw   = 0.0; // val0  - raw internal RDAC temperature ADC
    qreal coolantT     = 0.0; // val1  - coolant temperature (raw)
    qreal voltsRaw     = 0.0; // val2  - raw voltage ADC
    qreal fuelL2       = 0.0; // val3  - fuel level 2 (raw)
    qreal curr         = 0.0; // val4  - current (raw ADC)
    qreal fuelL1       = 0.0; // val5  - fuel level 1 / MAP input (raw)
    qreal fuelFlow1Raw = 0.0; // val6  - fuel flow 1 (raw pulses)
    qreal fuelP        = 0.0; // val7  - fuel pressure (raw ADC)
    qreal ax2          = 0.0; // val8  - auxiliary channel 2 (raw ADC)
    qreal ax1          = 0.0; // val9  - auxiliary channel 1 (raw ADC)
    qreal oilP         = 0.0; // val10 - oil pressure (raw ADC)
    qreal oilT         = 0.0; // val11 - oil temperature (raw ADC)
    qreal rpm1Raw      = 0.0; // val12 - RPM channel 1 (raw pulses)
    qreal iat          = 0.0; // val13 - internal air temperature (converted)
    qreal oat          = 0.0; // val14 - outside air temperature (converted)
    qreal volts        = 0.0; // val15 - battery voltage (converted)
    qreal current2     = 0.0; // val16 - current sensor 2 (converted amps)
    qreal current1     = 0.0; // val17 - current sensor 1 (converted amps)
    qreal manP         = 0.0; // val18 - manifold pressure (converted in Hg)
    qreal fuelFlow     = 0.0; // val19 - fuel flow (converted GPH)
    qreal fuelPress    = 0.0; // val20 - fuel pressure (converted PSI)
    qreal cht2         = 0.0; // val21 - CHT cylinder 2 (converted)
    qreal cht1         = 0.0; // val22 - CHT cylinder 1 (converted)
    qreal oilPress     = 0.0; // val23 - oil pressure (converted PSI)
    qreal oilTemp      = 0.0; // val24 - oil temperature (converted)
    qreal rpm1         = 0.0; // val25 - RPM channel 1 (converted)
    qreal tc1          = 0.0; // val26 - thermocouple 1 (converted)
    qreal tc2          = 0.0; // val27 - thermocouple 2 (converted)
    qreal tc3          = 0.0; // val28 - thermocouple 3 (converted)
    QDateTime lastMessageTime;
};

Q_DECLARE_METATYPE(EngineData)

#endif // ENGINEDATA_H
