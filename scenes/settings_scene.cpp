#include "scenes/settings_scene.h"

settingsScene::settingsScene(QObject* parent) :
    QGraphicsScene(parent)
{
    addItem(&userSet);

}
