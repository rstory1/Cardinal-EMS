#ifndef EMSSCENE_H
#define EMSSCENE_H

#include <QObject>
#include <QGraphicsScene>

#include <emsfull.h>

class emsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    emsScene(QObject* parent = 0);

private:
//    currentView;
    emsFull fullEMS;

};

#endif // EMSSCENE_H
