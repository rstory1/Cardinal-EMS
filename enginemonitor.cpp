#include "enginemonitor.h"

EngineMonitor::EngineMonitor(QWidget *parent) : QGraphicsView(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint);
}

EngineMonitor::~EngineMonitor()
{
}
