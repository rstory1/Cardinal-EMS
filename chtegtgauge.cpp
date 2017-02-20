//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2012 Tobias Rad                                        //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "chtegtgauge.h"

ChtEgt::ChtEgt(QGraphicsObject *parent) : QGraphicsObject(parent)
  , minChtValue(0.0)
  , maxChtValue(0.0)
  , greenYellowChtValue(0.0)
  , yellowRedChtValue(0.0)
{
    currentChtValues << 0.0 << 0.0 << 0.0 << 0.0;
    currentEgtValues << 0.0 << 0.0 << 0.0 << 0.0;
}

QRectF ChtEgt::boundingRect() const
{
    return QRectF(-240, -170, 340, 260);
}

void ChtEgt::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	//Set Clipping Rect
	painter->setClipRect(boundingRect());

	//Draw the side legend
	painter->setBrush(Qt::green);
	painter->setPen(QPen(Qt::green, 0));
    painter->drawRect(QRectF(QPointF(60.0, calculateLocalValue(minChtValue)), QPointF(90.0, calculateLocalValue(greenYellowChtValue))));
	painter->setBrush(Qt::yellow);
	painter->setPen(QPen(Qt::yellow, 0));
    painter->drawRect(QRectF(QPointF(60.0, calculateLocalValue(greenYellowChtValue)), QPointF(90.0, calculateLocalValue(yellowRedChtValue))));
	painter->setBrush(Qt::red);
	painter->setPen(QPen(Qt::red, 0));
    painter->drawRect(QRectF(QPointF(60.0, calculateLocalValue(yellowRedChtValue)), QPointF(90.0, calculateLocalValue(maxChtValue))));

	//Set painter for texts
	painter->setPen(QPen(Qt::white, 1));
    painter->setFont(QFont("Arial", 18));

	//Draw the static texts
    painter->drawText(QRectF(50.0, -135.0, 50.0, 20.0), Qt::AlignCenter | Qt::AlignVCenter, "CHT");
    painter->drawText(QRectF(-240.0, calculateLocalValue(minChtValue)+5, 50.0, 20.0), Qt::AlignCenter | Qt::AlignVCenter, "EGT");
    painter->drawText(QRectF(90.0, 40.0, 40.0, 20.0), Qt::AlignRight | Qt::AlignBottom, QString::fromUtf8("°C"));

	//Draw the ticks and numbers
	foreach(double value, betweenValues)
	{
		painter->drawLine(80, calculateLocalValue(value), 90, calculateLocalValue(value));
		painter->drawText(QRectF(90.0, calculateLocalValue(value)-10.0, 35.0, 20.0), Qt::AlignRight | Qt::AlignVCenter, QString::number(value, 'f', 0));
	}

	//Draw the red line to define warning range
	painter->setPen(Qt::red);
    painter->drawLine(-190, calculateLocalValue(yellowRedChtValue), 60, calculateLocalValue(yellowRedChtValue));

    //Draw the yellow line to define warning range
    painter->setPen(Qt::yellow);
    painter->drawLine(-190, calculateLocalValue(greenYellowChtValue), 60, calculateLocalValue(greenYellowChtValue));

    //Draw the white line to define base of bars
    painter->setPen(Qt::white);
    painter->drawLine(-190, calculateLocalValue(minChtValue)+2, 50, calculateLocalValue(minChtValue)+2);

	//Draw center dashed lines
//	painter->setPen(QPen(Qt::white, 1, Qt::DashLine));
//	for(int i = 0; i < 4; i++)
//	{
//        painter->drawLine(i*60-160, -120, i*60-160, 60);
//	}

	//Draw the bar graphes
	for(int i = 0; i < 4; i++)
	{
        QRectF barRect = QRectF(QPointF(i*60-180, 60), QPointF(i*60-140, calculateLocalValue(currentChtValues.value(i))));
        if(currentChtValues.at(i) > yellowRedChtValue)
		{
			//If value is in warning area, bar is drawn red
			painter->setBrush(Qt::red);
            cylinderAlarm = 3;
		}
        else if(currentChtValues.at(i) > greenYellowChtValue)
		{
			//If value is in caution area, bar is drawn yellow
			painter->setBrush(Qt::yellow);
            cylinderAlarm = 2;
		}
		else
		{
			//In all other cases, bar is drawn green
			painter->setBrush(Qt::green);
            cylinderAlarm = 1;
		}

        if((currentChtValues.at(i) > minChtValue) &&
                (currentChtValues.at(i) < maxChtValue))
		{
			//If value is in visible range, draw the bar
			painter->setPen(painter->brush().color());
			painter->drawRect(barRect);
		}
        else
        {
            painter->setPen(painter->brush().color());
            painter->drawRect(barRect);
//			//If value is outside the displayed range, draw a red cross
//			painter->setPen(QPen(Qt::red, 2));
//			painter->drawLine(barRect.left(), 60, barRect.right(), -120);
//			painter->drawLine(barRect.left(), -120, barRect.right(), 60);
        }

		if(painter->brush().color() == Qt::green)
		{
			//If value is in normal range, draw text in white
			painter->setPen(Qt::white);
		}

        //Define CHT text position and move to current column
        QRectF textRect(-35, -20, 40, 20);
        textRect.moveCenter(QPointF(i*60-160, -135));

//		if(i%2)
//		{
//			//All odd values should be raised
//			textRect.translate(QPointF(0.0, -20.0));
//		}


        //
        if ((isAlarmedRed == true) && (cylinderAlarm == 3)) {
            if (flashState) {
                painter->setPen(Qt::red);
                painter->setBrush(Qt::red);
                painter->drawRect(textRect);
                painter->setPen(Qt::white);
                painter->drawText(textRect, Qt::AlignCenter, QString::number(currentChtValues.at(i), 'f', 0));

            } else {
                painter->setPen(Qt::red);
                painter->drawText(textRect, Qt::AlignCenter, QString::number(currentChtValues.at(i), 'f', 0));
            }

        } else if ((isAlarmedYellow) && (cylinderAlarm == 2)) {
            if (flashState) {
                painter->setPen(Qt::yellow);
                painter->setBrush(Qt::yellow);
                painter->drawRect(textRect);
                painter->setPen(Qt::black);
                painter->drawText(textRect, Qt::AlignCenter, QString::number(currentChtValues.at(i), 'f', 0));

            } else {
                painter->setPen(Qt::yellow);
                painter->drawText(textRect, Qt::AlignCenter, QString::number(currentChtValues.at(i), 'f', 0));
            }
        } else {
            //Draw the readout
            painter->drawText(textRect, Qt::AlignCenter, QString::number(currentChtValues.at(i), 'f', 0));
        }

        //Define EGT text position and move to current column
        QRectF textRectEgt(-35, 65, 45, 20);
        textRectEgt.moveCenter(QPointF(i*60-155, 75));

//        if(i%2)
//        {
//            //All odd values should be raised
//            textRectEgt.translate(QPointF(0.0, 20.0));
//        }

        painter->drawText(textRectEgt, Qt::AlignCenter, QString::number(currentEgtValues.at(i), 'f', 0));

        update();
	}

}

double ChtEgt::calculateLocalValue(double value) const
{
    return -(value-minChtValue)/(maxChtValue-minChtValue)*180.0+60.0;
}

void ChtEgt::addBetweenValue(double value)
{
	betweenValues.append(value);
}

void ChtEgt::setChtValues(double val1, double val2, double val3, double val4)
{

    currentChtValues.replace(0, val1);
    currentChtValues.replace(1, val2);
    currentChtValues.replace(2, val3);
    currentChtValues.replace(3, val4);

    if ((((yellowRedChtValue > val1) && (val1 >= greenYellowChtValue))
         || ((yellowRedChtValue > val2) && (val2 >= greenYellowChtValue))
         || ((yellowRedChtValue > val3) && (val3 >= greenYellowChtValue))
         || ((yellowRedChtValue > val4) && (val4 >= greenYellowChtValue)))
            && (isAlarmedYellow == false && isAlarmedRed == false))
    {
        emit sendAlarm("CHT", Qt::yellow, true);
        isAlarmedYellow = true;
    }
    else if ((val1 >= yellowRedChtValue || val2 >= yellowRedChtValue || val3 >= yellowRedChtValue || val4 >= yellowRedChtValue) && (isAlarmedRed == false))
    {
        if (isAlarmedYellow)
        {
            emit cancelAlarm("CHT");
            isAlarmedYellow = false;

            emit sendAlarm("CHT", Qt::red, true);
            isAlarmedRed = true;
        }
        else
        {
            emit sendAlarm("CHT", Qt::red, true);
            isAlarmedRed = true;
        }

    }
    else if ((isAlarmedRed) && (val1 < yellowRedChtValue && val2 < yellowRedChtValue && val3 < yellowRedChtValue && val4 < yellowRedChtValue))
    {
        emit cancelAlarm("CHT");

        isAlarmedRed = false;
    }
    else if ((isAlarmedYellow) && (val1 < greenYellowChtValue && val2 < greenYellowChtValue && val3 < greenYellowChtValue && val4 < greenYellowChtValue))
    {
        emit cancelAlarm("CHT");

        isAlarmedYellow = false;
    }


}

void ChtEgt::setEgtValues(double val1, double val2, double val3, double val4)
{

    currentEgtValues.replace(0, val1);
    currentEgtValues.replace(1, val2);
    currentEgtValues.replace(2, val3);
    currentEgtValues.replace(3, val4);

}

void ChtEgt::setBorders(double minimum, double maximum, double yellowBorder, double redBorder)
{
    minChtValue = minimum;
    maxChtValue = maximum;
    greenYellowChtValue = yellowBorder;
    yellowRedChtValue = redBorder;
}

void ChtEgt::changeFlashState()
{
    if (flashState == false) {
        flashState  = true;
    } else {
        flashState = false;
    }
}
