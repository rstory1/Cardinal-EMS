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

QT       += core gui widgets serialport printsupport

INCLUDEPATH += \
    include \

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
    qcustomplot/qcustomplot.cpp \
    utilities/udpsocket.cpp \
    utilities/flightcalculator.cpp \
    instruments/windvector.cpp \
    instruments/hourmeter.cpp \
    utilities/spatial.cpp \
    utilities/gaugesettings.cpp \
    utilities/usersettings.cpp

HEADERS  += include/enginemonitor.h \
    include/bargraph.h \
    include/rpmindicator.h \
    include/cylinderheadtemperature.h \
    include/exhaustgastemperature.h \
    include/fuelmanagement.h \
    include/rdacconnect.h \
    include/nmeaconnect.h \
    include/manifoldpressure.h \
    include/sensorconvert.h \
    include/circulargauge.h \
    include/alarmBox.h \
    include/textBoxGauge.h \
    include/fueldisplay.h \
    include/chtegtgauge.h \
    include/buttonbar.h \
    include/qcustomplot.h \
    include/udpsocket.h \
    include/flightcalculator.h \
    include/windvector.h \
    include/hourmeter.h \
    include/spatial.h \
    include/gaugesettings.h \
    include/usersettings.h

RESOURCES += \
    res/res.qrc

#target.path = /Cardinal-EMS
#INSTALLS += target

#setting.path = /Cardinal-EMS/settings
#setting.files = /home/rstory/Avionics/Cardinal-EMS/settings/gaugeSettings.ini /home/rstory/Avionics/Cardinal-EMS/settings/settings.ini
#INSTALLS += setting

