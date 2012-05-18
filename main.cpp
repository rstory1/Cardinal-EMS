#include <QtGui/QApplication>
#include "enginemonitor.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EngineMonitor engineMonitor;
	engineMonitor.show();
	engineMonitor.move(0, 0);
	engineMonitor.resize(800, 600);
	return a.exec();
}
