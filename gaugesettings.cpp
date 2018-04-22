//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
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

#include "gaugesettings.h"

GaugeSettings::GaugeSettings(QObject *parent) : QObject(parent)
  , settings("settings/gaugeSettings.ini", QSettings::IniFormat)
{

}

void GaugeSettings::getGaugeDef() {
    gaugeDef myDef;  //Temporary 'gaugeDef'
    int i;

    for (i=0; i<NRange; i++) {
        myDef.start = settings.value(name + "/range" + QString::number(i+1) + "start",0).toDouble();
        myDef.end = settings.value(name + "/range" + QString::number(i+1) + "end",0).toDouble();
        myDef.color = QColor(settings.value(name + "/range" + QString::number(i+1) + "color","blue").toString());
        //qDebug() << name << myDef.start << myDef.end;
        definitions.push_back(myDef);
    }

    if (name == "RPM") {
        for (i=0; i<warmupNRange; i++) {
            myDef.start = settings.value(name + "/warmuprange" + QString::number(i+1) + "start",0).toDouble();
            myDef.end = settings.value(name + "/warmuprange" + QString::number(i+1) + "end",0).toDouble();
            myDef.color = QColor(settings.value(name + "/warmuprange" + QString::number(i+1) + "color","blue").toString());

            warmupDefinitions.push_back(myDef);
        }
    }

}

void GaugeSettings::setGauge(QString gauge) {
    name = gauge;

    NRange = settings.value(name + "/NRange",0).toInt();

    if (name == "RPM") {
        warmupNRange = settings.value(name + "/warmupNRange",0).toInt();
    } else if (name == "CHT") {
        egtMin = settings.value("EGT/min",0).toDouble();
        egtMax = settings.value("EGT/max",0).toDouble();
    }

    min = settings.value(name + "/min",0).toDouble();
    max = settings.value(name + "/max",0).toDouble();

    getGaugeDef();
}
