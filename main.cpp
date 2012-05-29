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

#include <QtGui/QApplication>
#include "enginemonitor.h"

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

	//Create splashscreen and show it
	QPixmap pixmap(":/splashscreen.png");
	QSplashScreen splash(pixmap);
	splash.show();
	a.processEvents();

	//Create the engine monitor and show after splashscreen delay
	EngineMonitor engineMonitor;
	SplashScreenDelay::sleep(5);
	engineMonitor.show();
	engineMonitor.move(0, 0);
	engineMonitor.resize(800, 480);
	splash.finish(&engineMonitor);

	//Alternative for other resolutions
//	engineMonitor.showFullScreen();
//	engineMonitor.scale(1.28, 1.28);
	return a.exec();
}
