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
    qreal intTempRaw;   // val0  - raw internal RDAC temperature ADC
    qreal coolantT;     // val1  - coolant temperature (raw)
    qreal voltsRaw;     // val2  - raw voltage ADC
    qreal fuelL2;       // val3  - fuel level 2 (raw)
    qreal curr;         // val4  - current (raw ADC)
    qreal fuelL1;       // val5  - fuel level 1 / MAP input (raw)
    qreal fuelFlow1Raw; // val6  - fuel flow 1 (raw pulses)
    qreal fuelP;        // val7  - fuel pressure (raw ADC)
    qreal ax2;          // val8  - auxiliary channel 2 (raw ADC)
    qreal ax1;          // val9  - auxiliary channel 1 (raw ADC)
    qreal oilP;         // val10 - oil pressure (raw ADC)
    qreal oilT;         // val11 - oil temperature (raw ADC)
    qreal rpm1Raw;      // val12 - RPM channel 1 (raw pulses)
    qreal iat;          // val13 - internal air temperature (converted)
    qreal oat;          // val14 - outside air temperature (converted)
    qreal volts;        // val15 - battery voltage (converted)
    qreal current2;     // val16 - current sensor 2 (converted amps)
    qreal current1;     // val17 - current sensor 1 (converted amps)
    qreal manP;         // val18 - manifold pressure (converted in Hg)
    qreal fuelFlow;     // val19 - fuel flow (converted GPH)
    qreal fuelPress;    // val20 - fuel pressure (converted PSI)
    qreal cht2;         // val21 - CHT cylinder 2 (converted)
    qreal cht1;         // val22 - CHT cylinder 1 (converted)
    qreal oilPress;     // val23 - oil pressure (converted PSI)
    qreal oilTemp;      // val24 - oil temperature (converted)
    qreal rpm1;         // val25 - RPM channel 1 (converted)
    qreal tc1;          // val26 - thermocouple 1 (converted)
    qreal tc2;          // val27 - thermocouple 2 (converted)
    qreal tc3;          // val28 - thermocouple 3 (converted)
    QDateTime lastMessageTime;
};

Q_DECLARE_METATYPE(EngineData)

#endif // ENGINEDATA_H
