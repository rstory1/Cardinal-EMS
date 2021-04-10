//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2012 Tobias Rad                                        //
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

#include <QtWidgets>
#include <QApplication>
#include "enginemonitor.h"
#include "nmeaconnect.h"
#include "udpsocket.h"
#include "flightcalculator.h"
#include "spatial.h"

void messageToFileHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QDir dir(QApplication::applicationDirPath() + "/ems/appLogs");
    if (!dir.exists())
        dir.mkpath(".");
    QFile debugfile(QApplication::applicationDirPath() + "/ems/appLogs/EngineMon_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".log");
	if(debugfile.open(QIODevice::Append | QIODevice::Text))
	{
		QString debugString = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz").append(' ');
		switch (type)
		{
        case QtInfoMsg:
            debugString.append("Info: ");
            break;
		case QtDebugMsg:
			debugString.append("Debug: ");
			break;
		case QtWarningMsg:
			debugString.append("Warning: ");
			break;
		case QtCriticalMsg:
			debugString.append("Critical: ");
			break;
		case QtFatalMsg:
			debugString.append("Fatal: ");
			abort();
        }
        qInfo() << msg;
		debugfile.write(debugString.append(msg).replace('\n', ", ").append('\n').toLatin1());
		debugfile.close();
	}
	else
	{
		QMessageBox::warning(NULL, "No debug output", "Unable to open 'EngineMon.log', therefore no debug output available.");
	}
}

class SplashScreenDelay : public QThread
{
public:
	static void sleep(unsigned long secs)
	{
		QThread::sleep(secs);
	}
};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    QApplication::setOverrideCursor(Qt::BlankCursor);

    QApplication::setOrganizationName("Cardinal Avionics");
    QApplication::setApplicationName("Cardinal-EMS");

    qInfo() << "Before app log starts";

    QDir dir(QApplication::applicationDirPath() + "/ems/appLogs");
    if (!dir.exists())
        dir.mkpath(".");
    QFile debugfile(QApplication::applicationDirPath() + "/ems/appLogs/EngineMon_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".log");
    if(debugfile.open(QIODevice::Append | QIODevice::Text))
    {
		debugfile.write(QString("EngineMonitor started at: ").append(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz")).append('\n').toLatin1());
        debugfile.close();
        qInstallMessageHandler(messageToFileHandler);
	}
	else
	{
		QMessageBox::warning(NULL, "No debug output", "Unable to open 'EngineMon.log', therefore no debug output available.");
    }

	a.processEvents();

    QFont font;
    font.setFamily("Bitstream Vera Sans");
    a.setFont(font);


	//Create the engine monitor and show after splashscreen delay
    EngineMonitor engineMonitor;
    engineMonitor.move(0, 0);
    engineMonitor.resize(800, 480);
    engineMonitor.setMaximumWidth(800);
    engineMonitor.setMaximumHeight(480);
    engineMonitor.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    engineMonitor.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    engineMonitor.show();

	return a.exec();
}
