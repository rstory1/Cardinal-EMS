#ifndef INSTRUMENTSETTINGS_H
#define INSTRUMENTSETTINGS_H

#include <QWidget>
#include <QtWidgets>
#include <QObject>

class instrumentSettings : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit instrumentSettings(QGraphicsObject* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    
private:
    quint16 analogValue;
    QString units;
    quint16 zeroRef; //  This is a value set when current through a shunt or measuring device is zero for reference
    
public slots:
    void onAdcUpdate(quint16 adcValue);
};

#endif // INSTRUMENTSETTINGS_H
