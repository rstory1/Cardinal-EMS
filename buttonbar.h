#ifndef BUTTONBAR_H
#define BUTTONBAR_H

#include <QtWidgets>

class ButtonBar : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ButtonBar(QGraphicsObject * parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    int numOfButtons = 4;
    int buttonLocation = 1; // 1-Bottom; 2-Top; 3-Left Side; 4-Right Side
    int buttonDisplay = 1; //1-Menu; 2-Fuel; 3-Settings
};

#endif // BUTTONBAR_H
