#ifndef BUTTONBAR_H
#define BUTTONBAR_H

#include <QtWidgets>
#include <QFile>

//! Button Bar Class
/*!
 * This class creates a row of buttons to facilitate user interaction
*/

class ButtonBar : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ButtonBar(QGraphicsObject * parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QRectF buttonRect1 = QRectF(0,-50.0,100,50);
    QRectF buttonRect2 = QRectF(100,-50.0,100,50);
    QRectF buttonRect3 = QRectF(200,-50.0,100,50);
    QRectF buttonRect4 = QRectF(300,-50.0,100,50);
    QRectF buttonRect5 = QRectF(400,-50.0,100,50);
    QRectF buttonRect6 = QRectF(500,-50.0,100,50);
    QPushButton button1;

    int buttonWidth;
    int buttonHeight;
    int numOfButtons = 5; /*!< Number of buttons to be drawn */
    int buttonLocation = 1; /*!< Possible Values: 1-Bottom; 2-Top; 3-Left Side; 4-Right Side */
    int buttonDisplay = 1; /*!< Possible Values: 1-Menu; 2-Fuel; 3-Settings */

    QLinearGradient gradient1 = QLinearGradient(buttonRect1.topLeft(), QPointF(0,0.7*buttonRect1.top()));
    QLinearGradient gradient2 = QLinearGradient(buttonRect2.topLeft(), QPointF(buttonRect2.left(),0.7*buttonRect2.top()));
    QLinearGradient gradient3 = QLinearGradient(buttonRect3.topLeft(), QPointF(buttonRect3.left(),0.7*buttonRect3.top()));
    QLinearGradient gradient4 = QLinearGradient(buttonRect4.topLeft(), QPointF(buttonRect4.left(),0.7*buttonRect4.top()));
    QLinearGradient gradient5 = QLinearGradient(buttonRect5.topLeft(), QPointF(buttonRect5.left(),0.7*buttonRect5.top()));

    bool isAlarmFlashing = false;

    void ackPressed();
    void quitPressed() {
        QApplication::quit();
    }


signals:
    void sendAlarmAck();
    void sendFuelChange(QString changeDirection); //  + or -
    void switchScene(int scene);

public slots:
    void onAlarmFlash(); /*!< This slot is entered when a flashing alarm is created */
    void onNoFlashingAlarm();
};

#endif // BUTTONBAR_H
