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

QT       += core gui widgets multimedia #svg

TARGET = EngineMonitor
TEMPLATE = app

DESTDIR = ../bin

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
    PortListener.cpp \
    sensorconvert.cpp \
    circulargauge.cpp \
    alarmBox.cpp \
    textBoxGauge.cpp \
    fueldisplay.cpp \
    chtegtgauge.cpp \
    buttonbar.cpp \
    qcustomplot/qcustomplot.cpp \
    wiringPi/devLib/ds1302.c \
    wiringPi/devLib/gertboard.c \
    wiringPi/devLib/lcd.c \
    wiringPi/devLib/lcd128x64.c \
    wiringPi/devLib/maxdetect.c \
    wiringPi/devLib/piFace.c \
    wiringPi/devLib/piFaceOld.c \
    wiringPi/devLib/piGlow.c \
    wiringPi/devLib/piNes.c \
    wiringPi/devLib/scrollPhat.c \
    wiringPi/examples/Gertboard/7segments.c \
    wiringPi/examples/Gertboard/buttons.c \
    wiringPi/examples/Gertboard/gertboard.c \
    wiringPi/examples/Gertboard/record.c \
    wiringPi/examples/Gertboard/temperature.c \
    wiringPi/examples/Gertboard/voltmeter.c \
    wiringPi/examples/Gertboard/vumeter.c \
    wiringPi/examples/PiFace/blink.c \
    wiringPi/examples/PiFace/buttons.c \
    wiringPi/examples/PiFace/ladder.c \
    wiringPi/examples/PiFace/metro.c \
    wiringPi/examples/PiFace/motor.c \
    wiringPi/examples/PiFace/reaction.c \
    wiringPi/examples/PiGlow/piglow.c \
    wiringPi/examples/PiGlow/piGlow0.c \
    wiringPi/examples/PiGlow/piGlow1.c \
    wiringPi/examples/q2w/binary.c \
    wiringPi/examples/q2w/blink-io.c \
    wiringPi/examples/q2w/blink.c \
    wiringPi/examples/q2w/bright.c \
    wiringPi/examples/q2w/button.c \
    wiringPi/examples/q2w/volts.c \
    wiringPi/examples/scrollPhat/scphat.c \
    wiringPi/examples/scrollPhat/test.c \
    wiringPi/examples/blink.c \
    wiringPi/examples/blink12.c \
    wiringPi/examples/blink12drcs.c \
    wiringPi/examples/blink6drcs.c \
    wiringPi/examples/blink8.c \
    wiringPi/examples/clock.c \
    wiringPi/examples/delayTest.c \
    wiringPi/examples/ds1302.c \
    wiringPi/examples/isr-osc.c \
    wiringPi/examples/isr.c \
    wiringPi/examples/lcd-adafruit.c \
    wiringPi/examples/lcd.c \
    wiringPi/examples/lowPower.c \
    wiringPi/examples/max31855.c \
    wiringPi/examples/nes.c \
    wiringPi/examples/okLed.c \
    wiringPi/examples/pwm.c \
    wiringPi/examples/rht03.c \
    wiringPi/examples/serialRead.c \
    wiringPi/examples/serialTest.c \
    wiringPi/examples/servo.c \
    wiringPi/examples/softPwm.c \
    wiringPi/examples/softTone.c \
    wiringPi/examples/speed.c \
    wiringPi/examples/spiSpeed.c \
    wiringPi/examples/wfi.c \
    wiringPi/gpio/gpio.c \
    wiringPi/gpio/pins.c \
    wiringPi/gpio/readall.c \
    wiringPi/wiringPi/ads1115.c \
    wiringPi/wiringPi/drcSerial.c \
    wiringPi/wiringPi/max31855.c \
    wiringPi/wiringPi/max5322.c \
    wiringPi/wiringPi/mcp23008.c \
    wiringPi/wiringPi/mcp23016.c \
    wiringPi/wiringPi/mcp23017.c \
    wiringPi/wiringPi/mcp23s08.c \
    wiringPi/wiringPi/mcp23s17.c \
    wiringPi/wiringPi/mcp3002.c \
    wiringPi/wiringPi/mcp3004.c \
    wiringPi/wiringPi/mcp3422.c \
    wiringPi/wiringPi/mcp4802.c \
    wiringPi/wiringPi/pcf8574.c \
    wiringPi/wiringPi/pcf8591.c \
    wiringPi/wiringPi/piHiPri.c \
    wiringPi/wiringPi/piThread.c \
    wiringPi/wiringPi/sn3218.c \
    wiringPi/wiringPi/softPwm.c \
    wiringPi/wiringPi/softServo.c \
    wiringPi/wiringPi/softTone.c \
    wiringPi/wiringPi/sr595.c \
    wiringPi/wiringPi/wiringPi.c \
    wiringPi/wiringPi/wiringPiI2C.c \
    wiringPi/wiringPi/wiringPiSPI.c \
    wiringPi/wiringPi/wiringSerial.c \
    wiringPi/wiringPi/wiringShift.c \
    wiringPi/wiringPi/wpiExtensions.c

HEADERS  += enginemonitor.h \
    bargraph.h \
    rpmindicator.h \
    cylinderheadtemperature.h \
    exhaustgastemperature.h \
    fuelmanagement.h \
    rdacconnect.h \
    nmeaconnect.h \
    manifoldpressure.h \
    PortListener.h \
    sensorconvert.h \
    circulargauge.h \
    alarmBox.h \
    textBoxGauge.h \
    fueldisplay.h \
    chtegtgauge.h \
    buttonbar.h \
    qcustomplot/qcustomplot.h \
    wiringPi/devLib/ds1302.h \
    wiringPi/devLib/font.h \
    wiringPi/devLib/gertboard.h \
    wiringPi/devLib/lcd.h \
    wiringPi/devLib/lcd128x64.h \
    wiringPi/devLib/maxdetect.h \
    wiringPi/devLib/piFace.h \
    wiringPi/devLib/piGlow.h \
    wiringPi/devLib/piNes.h \
    wiringPi/devLib/scrollPhat.h \
    wiringPi/devLib/scrollPhatFont.h \
    wiringPi/examples/header.h \
    wiringPi/gpio/version.h \
    wiringPi/wiringPi/ads1115.h \
    wiringPi/wiringPi/drcSerial.h \
    wiringPi/wiringPi/max31855.h \
    wiringPi/wiringPi/max5322.h \
    wiringPi/wiringPi/mcp23008.h \
    wiringPi/wiringPi/mcp23016.h \
    wiringPi/wiringPi/mcp23016reg.h \
    wiringPi/wiringPi/mcp23017.h \
    wiringPi/wiringPi/mcp23s08.h \
    wiringPi/wiringPi/mcp23s17.h \
    wiringPi/wiringPi/mcp23x08.h \
    wiringPi/wiringPi/mcp23x0817.h \
    wiringPi/wiringPi/mcp3002.h \
    wiringPi/wiringPi/mcp3004.h \
    wiringPi/wiringPi/mcp3422.h \
    wiringPi/wiringPi/mcp4802.h \
    wiringPi/wiringPi/pcf8574.h \
    wiringPi/wiringPi/pcf8591.h \
    wiringPi/wiringPi/sn3218.h \
    wiringPi/wiringPi/softPwm.h \
    wiringPi/wiringPi/softServo.h \
    wiringPi/wiringPi/softTone.h \
    wiringPi/wiringPi/sr595.h \
    wiringPi/wiringPi/wiringPi.h \
    wiringPi/wiringPi/wiringPiI2C.h \
    wiringPi/wiringPi/wiringPiSPI.h \
    wiringPi/wiringPi/wiringSerial.h \
    wiringPi/wiringPi/wiringShift.h \
    wiringPi/wiringPi/wpiExtensions.h

RESOURCES += \
    res/res.qrc

LIBS += -L~/Avionics/ems/wiringPi -lwiringPi

include(./qextserialport/qextserialport.pri)
