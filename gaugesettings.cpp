#include "gaugesettings.h"

GaugeSettings::GaugeSettings(QObject *parent) : QObject(parent)
  , settings("settings/gaugeSettings.ini", QSettings::IniFormat)
{

}

void GaugeSettings::getGaugeDef() {
    gaugeDef myDef;  //Temporary 'gaugeDef'
    int i;

    for (i=0; i<NRange; i++) {
        myDef.start = settings.value(name + "/range" + QString::number(i+1) + "start",0).toInt();
        myDef.end = settings.value(name + "/range" + QString::number(i+1) + "end",0).toInt();
        myDef.color = QColor(settings.value(name + "/range" + QString::number(i+1) + "color","blue").toString());

        definitions.push_back(myDef);
    }

    if (name == "RPM") {
        for (i=0; i<warmupNRange; i++) {
            myDef.start = settings.value(name + "/warmuprange" + QString::number(i+1) + "start",0).toInt();
            myDef.end = settings.value(name + "/warmuprange" + QString::number(i+1) + "end",0).toInt();
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
    }

    getGaugeDef();
}
