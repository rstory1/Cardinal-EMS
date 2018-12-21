#ifndef DIAGNOSTICWIDGET_H
#define DIAGNOSTICWIDGET_H

#include <QtWidgets>

class diagnosticWidget : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit diagnosticWidget(QGraphicsObject* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
};

#endif // DIAGNOSTICWIDGET_H
