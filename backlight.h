#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <QObject>
#include <QtCore>
#include <QtWidgets>
#include <QString>
#include <fstream>

class backlight : public QGraphicsObject
{
public:
    explicit backlight(QGraphicsObject * parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setBrightness(int brightnessPercent);

private:
    int brightness;

    int boundingX = -50;
    int boundingY = -100;
    int boundingWidth = 100;
    int boundingHeight = 150;

};

#endif // BACKLIGHT_H
