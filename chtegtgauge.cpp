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
  , minEgtValue(0.0)
  , maxEgtValue(0.0)
{
    currentChtValues << 0.0 << 0.0 << 0.0 << 0.0;
    currentEgtValues << 0.0 << 0.0 << 0.0 << 0.0;
}

QRectF ChtEgt::boundingRect() const
{
    return QRectF(-240, -170, 340, 190);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void ChtEgt::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	//Set Clipping Rect
	painter->setClipRect(boundingRect());

	//Draw the side legend
	painter->setBrush(Qt::green);
	painter->setPen(QPen(Qt::green, 0));
    painter->drawRect(QRectF(QPointF(60.0, calculateLocalChtValue(minChtValue)), QPointF(90.0, calculateLocalChtValue(greenYellowChtValue))));
	painter->setBrush(Qt::yellow);
	painter->setPen(QPen(Qt::yellow, 0));
    painter->drawRect(QRectF(QPointF(60.0, calculateLocalChtValue(greenYellowChtValue)), QPointF(90.0, calculateLocalChtValue(yellowRedChtValue))));
	painter->setBrush(Qt::red);
	painter->setPen(QPen(Qt::red, 0));
    painter->drawRect(QRectF(QPointF(60.0, calculateLocalChtValue(yellowRedChtValue)), QPointF(90.0, calculateLocalChtValue(maxChtValue))));

	//Set painter for texts
	painter->setPen(QPen(Qt::white, 1));
    painter->setFont(QFont("Arial", 16));

	//Draw the static texts
    QRectF chtTitleRect = QRectF(50.0, calculateLocalChtValue(maxChtValue)-25, 50.0, 20.0);
    painter->drawText(QRectF(-240.0, calculateLocalChtValue(minChtValue)+7, 50.0, 20.0), Qt::AlignCenter | Qt::AlignVCenter, "EGT");
    painter->drawText(QRectF(-240.0, calculateLocalChtValue(maxChtValue)-25, 50.0, 20.0), Qt::AlignCenter | Qt::AlignBottom, QString::fromUtf8("°F"));


    //Set painter for texts
    painter->setPen(QPen(Qt::white, 1));
    painter->setFont(QFont("Arial", 18, QFont::Bold));

	//Draw the red line to define warning range
	painter->setPen(Qt::red);
    painter->drawLine(-190, calculateLocalChtValue(yellowRedChtValue), 60, calculateLocalChtValue(yellowRedChtValue));

    //Draw the yellow line to define warning range
    painter->setPen(Qt::yellow);
    painter->drawLine(-190, calculateLocalChtValue(greenYellowChtValue), 60, calculateLocalChtValue(greenYellowChtValue));

    //Draw the white line to define base of bars
    painter->setPen(Qt::white);
    painter->drawLine(-210, calculateLocalChtValue(minChtValue)+2, 50, calculateLocalChtValue(minChtValue)+2);

    //Draw the white line to define top of bars
    painter->setPen(Qt::white);
    painter->drawLine(-190, calculateLocalChtValue(maxChtValue)-2, 90, calculateLocalChtValue(maxChtValue)-2);

    //Draw the white line to show EGT scale
    painter->setPen(Qt::white);
    painter->drawLine(-210, calculateLocalChtValue(maxChtValue)-2, -200, calculateLocalChtValue(maxChtValue)-2);
    painter->drawLine(-210, calculateLocalChtValue(minChtValue)+2, -210, calculateLocalChtValue(maxChtValue)-2);

    //Draw center lines
    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine));
    for(int i = 0; i < 4; i++)
    {
        painter->drawLine(i*60-160, -110, i*60-160, -10);
    }

	//Draw the bar graphes
	for(int i = 0; i < 4; i++)
	{
        QRectF barRect = QRectF(QPointF(i*60-180, -10), QPointF(i*60-140, calculateLocalChtValue(currentChtValues.value(i))));
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
        else if (currentChtValues.at(i) > maxChtValue)
        {
            barRect = QRectF(QPointF(i*60-180, -10), QPointF(i*60-140, calculateLocalChtValue(maxChtValue)));
            painter->setPen(painter->brush().color());
            painter->drawRect(barRect);
        }

		if(painter->brush().color() == Qt::green)
		{
			//If value is in normal range, draw text in white
			painter->setPen(Qt::white);
		}

        //Define CHT text position and move to current column
        QRectF textRect(-40, -20, 50, 20);
        textRect.moveCenter(QPointF(i*60-160, -125));

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

        } else if ((cylinderAlarm == 2)) {
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
            painter->setPen(Qt::white);
            painter->drawText(textRect, Qt::AlignCenter, QString::number(currentChtValues.at(i), 'f', 0));
        }

        //Define EGT text position and move to current column
        painter->setPen(Qt::white);
        QRectF textRectEgt(-45, 65, 65, 20);
        textRectEgt.moveCenter(QPointF(i*60-160, 5));

        painter->drawText(textRectEgt, Qt::AlignCenter, QString::number(currentEgtValues.at(i), 'f', 0));

        //  Draw the markers for the EGT gauge
        QRectF EgtRect = QRectF(QPointF(i*60-185, calculateLocalEgtValue(currentEgtValues.value(i))-3), QPointF(i*60-135, calculateLocalEgtValue(currentEgtValues.value(i))+3));

        if((currentEgtValues.at(i) > minEgtValue) &&
                (currentEgtValues.at(i) < maxEgtValue))
        {

            QPolygonF marker1;
            marker1.append(QPointF(i*60-190, calculateLocalEgtValue(currentEgtValues.value(i))+10));
            marker1.append(QPointF(i*60-180, calculateLocalEgtValue(currentEgtValues.value(i))));
            marker1.append(QPointF(i*60-190, calculateLocalEgtValue(currentEgtValues.value(i))-10));

            QPolygonF marker2;
            marker2.append(QPointF(i*60-130, calculateLocalEgtValue(currentEgtValues.value(i))+10));
            marker2.append(QPointF(i*60-140, calculateLocalEgtValue(currentEgtValues.value(i))));
            marker2.append(QPointF(i*60-130, calculateLocalEgtValue(currentEgtValues.value(i))-10));


            painter->setPen(Qt::black);
            painter->setBrush(Qt::white);
            painter->drawRect(EgtRect);
            painter->setPen(Qt::white);
            //painter->drawPolygon(marker1);
            //painter->drawPolygon(marker2);
        }



        //update();
	}

    if ((isAlarmedRed == true)) {
        if (flashState) {
            painter->setPen(Qt::red);
            painter->setBrush(Qt::red);
            painter->drawRect(chtTitleRect);
            painter->setPen(Qt::white);
            painter->setFont(QFont("Arial", 16));
            painter->drawText(chtTitleRect, Qt::AlignCenter | Qt::AlignVCenter, "CHT");

        } else {
            painter->setPen(Qt::red);
            painter->setFont(QFont("Arial", 16));
            painter->drawText(chtTitleRect, Qt::AlignCenter | Qt::AlignVCenter, "CHT");
        }

    } else if ((isAlarmedYellow)) {
        if (flashState) {
            painter->setPen(Qt::yellow);
            painter->setBrush(Qt::yellow);
            painter->drawRect(chtTitleRect);
            painter->setFont(QFont("Arial", 16));
            painter->setPen(Qt::black);
            painter->drawText(chtTitleRect, Qt::AlignCenter | Qt::AlignVCenter, "CHT");

        } else {
            painter->setPen(Qt::yellow);
            painter->setFont(QFont("Arial", 16));
            painter->drawText(chtTitleRect, Qt::AlignCenter | Qt::AlignVCenter, "CHT");
        }
    } else {
        painter->setFont(QFont("Arial", 16));
        painter->drawText(chtTitleRect, Qt::AlignCenter | Qt::AlignVCenter, "CHT");
    }

    update();

}

double ChtEgt::calculateLocalChtValue(double value) const
{
    return -(value-minChtValue)/(maxChtValue-minChtValue)*100.0+(-10.0);
}

double ChtEgt::calculateLocalEgtValue(double value) const
{
    return -(value-minEgtValue)/(maxEgtValue-minEgtValue)*100.0+(-10.0);
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

void ChtEgt::setBorders(double minimum, double maximum, double yellowBorder, double redBorder, double minEgt, double maxEgt)
{
    minChtValue = minimum;
    maxChtValue = maximum;
    greenYellowChtValue = yellowBorder;
    yellowRedChtValue = redBorder;
    minEgtValue = minEgt;
    maxEgtValue = maxEgt;
}

void ChtEgt::changeFlashState()
{
    if (flashState == false) {
        flashState  = true;
    } else {
        flashState = false;
    }
}
