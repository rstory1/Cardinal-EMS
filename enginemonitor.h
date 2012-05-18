#ifndef ENGINEMONITOR_H
#define ENGINEMONITOR_H

#include <QtGui>

class EngineMonitor : public QGraphicsView
{
	Q_OBJECT
public:
	EngineMonitor(QWidget *parent = 0);
	~EngineMonitor();
};

#endif // ENGINEMONITOR_H
