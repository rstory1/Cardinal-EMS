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

QT       += core gui widgets serialport printsupport network

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
    bargraph.cpp \
    rpmindicator.cpp \
    cylinderheadtemperature.cpp \
    exhaustgastemperature.cpp \
    fuelmanagement.cpp \
    rdacconnect.cpp \
    nmeaconnect.cpp \
    manifoldpressure.cpp \
    sensorconvert.cpp \
    circulargauge.cpp \
    alarmBox.cpp \
    textBoxGauge.cpp \
    fueldisplay.cpp \
    chtegtgauge.cpp \
    buttonbar.cpp \
    qcustomplot/qcustomplot.cpp \
    udpsocket.cpp \
    flightcalculator.cpp \
    windvector.cpp \
    hourmeter.cpp \
    spatial.cpp \
    gaugesettings.cpp \
    dataserver.cpp \
    datathread.cpp

HEADERS  += enginemonitor.h \
    bargraph.h \
    rpmindicator.h \
    cylinderheadtemperature.h \
    exhaustgastemperature.h \
    fuelmanagement.h \
    rdacconnect.h \
    nmeaconnect.h \
    manifoldpressure.h \
    sensorconvert.h \
    circulargauge.h \
    alarmBox.h \
    textBoxGauge.h \
    fueldisplay.h \
    chtegtgauge.h \
    buttonbar.h \
    qcustomplot/qcustomplot.h \
    udpsocket.h \
    flightcalculator.h \
    windvector.h \
    hourmeter.h \
    spatial.h \
    gaugesettings.h \
    dataserver.h \
    datathread.h

RESOURCES += \
    res/res.qrc

#LIBS += -L~/Avionics/ems/wiringPi -lwiringPi
