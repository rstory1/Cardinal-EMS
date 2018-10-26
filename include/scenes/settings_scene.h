#ifndef SETTINGS_SCENE_H
#define SETTINGS_SCENE_H

#include <QObject>
#include <QGraphicsScene>

#include <userSettings.h>

class settingsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    settingsScene(QObject* parent = 0);

private:
    userSettings userSet;
};

#endif // SETTINGS_SCENE_H
