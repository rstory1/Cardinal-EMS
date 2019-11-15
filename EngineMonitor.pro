########################################################################
#                                                                      #
# EngineMonitor, a graphical gauge to monitor an aircraft's engine     #
# Copyright (C) 2012 Tobias Rad                                        #
#                                                                      #
# This program is free software: you can redistribute it and/or modify #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# This program is distributed in the hope that it will be useful,      #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with this program. If not, see <http://www.gnu.org/licenses/>. #
#                                                                      #
########################################################################

QT       += core gui widgets serialport

INCLUDEPATH += \
    include \
    include/instruments

TARGET = EngineMonitor
TEMPLATE = app

Release:DESTDIR = release
Release:OBJECTS_DIR = release/.obj
Release:MOC_DIR = release/.moc
Release:RCC_DIR = release/.rcc
Release:UI_DIR = release/.ui

Debug:DESTDIR = debug
Debug:OBJECTS_DIR = debug/.obj
Debug:MOC_DIR = debug/.moc
Debug:RCC_DIR = debug/.rcc
Debug:UI_DIR = debug/.ui

RC_FILE = ./res/icon.rc

SOURCES += main.cpp\
    enginemonitor.cpp \
    qdevicewatcher/qdevicewatcher.cpp \
    qdevicewatcher/qdevicewatcher_linux.cpp \
    utilities/datasmoothing.cpp \
    instruments/bargraph.cpp \
    instruments/rpmindicator.cpp \
    instruments/cylinderheadtemperature.cpp \
    instruments/exhaustgastemperature.cpp \
    instruments/fuelmanagement.cpp \
    utilities/rdacconnect.cpp \
    utilities/nmeaconnect.cpp \
    instruments/manifoldpressure.cpp \
    utilities/sensorconvert.cpp \
    instruments/circulargauge.cpp \
    instruments/alarmBox.cpp \
    instruments/textBoxGauge.cpp \
    instruments/fueldisplay.cpp \
    instruments/chtegtgauge.cpp \
    utilities/buttonbar.cpp \
    utilities/udpsocket.cpp \
    utilities/flightcalculator.cpp \
    instruments/windvector.cpp \
    instruments/hourmeter.cpp \
    utilities/spatial.cpp \
    utilities/gaugesettings.cpp \
    utilities/userSettings.cpp \
    scenes/settings_scene.cpp \
    scenes/emsfull.cpp \
    utilities/instrumentsettings.cpp
    instruments/diagnosticwidget.cpp
    qdevicewatcher/qdevicewatcher.cpp


HEADERS  += include/enginemonitor.h \
    include/datasmoothing.h \
    include/instruments/bargraph.h \
    include/instruments/rpmindicator.h \
    include/instruments/cylinderheadtemperature.h \
    include/instruments/exhaustgastemperature.h \
    include/instruments/fuelmanagement.h \
    include/qdevicewatcher/qdevicewatcher.h \
    include/qdevicewatcher/qdevicewatcher_p.h \
    include/rdacconnect.h \
    include/nmeaconnect.h \
    include/instruments/manifoldpressure.h \
    include/sensorconvert.h \
    include/instruments/circulargauge.h \
    include/alarmBox.h \
    include/instruments/textBoxGauge.h \
    include/instruments/fueldisplay.h \
    include/instruments/chtegtgauge.h \
    include/buttonbar.h \
    include/udpsocket.h \
    include/flightcalculator.h \
    include/instruments/windvector.h \
    include/instruments/hourmeter.h \
    include/spatial.h \
    include/gaugesettings.h \
    include/userSettings.h \
    include/scenes/settings_scene.h \
    include/scenes/emsfull.h \
    include/instrumentsettings.h
    include/diagnosticwidget.h
    include/qdevicewatcher/qdevicewatcher.h
    include/qdevicewatcher/qdevicewatcher_p.h

RESOURCES += \
    res/res.qrc

#target.path = /Cardinal-EMS
#INSTALLS += target

#setting.path = /Cardinal-EMS/settings
#setting.files = /home/rstory/Avionics/Cardinal-EMS/settings/gaugeSettings.ini /home/rstory/Avionics/Cardinal-EMS/settings/settings.ini
#INSTALLS += setting

