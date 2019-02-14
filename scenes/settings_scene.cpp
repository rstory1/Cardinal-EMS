#include "scenes/settings_scene.h"

settingsScene::settingsScene(QObject* parent) :
    QGraphicsScene(parent)
{
    addItem(&userSet);

    connect(&userSet, SIGNAL(zeroCurrent()), this, SLOT(onZeroCurrent()));

}

QRectF settingsScene::boundingRect() const
{
    return QRectF(0,0,800,480);
}

void settingsScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //Set Clipping Rect
    painter->setClipRect(boundingRect());

    if(isActive()) {
        update();
    }

}
