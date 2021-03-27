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

#include "textBoxGauge.h"

TextBox::TextBox(QGraphicsObject *parent)
	: QGraphicsObject(parent)
	, titleText("HEAD")
	, unitText("UNIT")
	, minValue(0.0)
	, maxValue(0.0)
	, currentValue(0.0)
	, barPrecision(0)
	, readoutPrecision(0)
{
    smoothData.setSampleSize(20);
}

QRectF TextBox::boundingRect() const
{
    return QRectF(-30, -30, 60, 60);

}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void TextBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);

    font.setFamily("Arial");
    font.setBold(false);
    font.setPointSize(12);
    painter->setFont(font);

	//Save thje painter and deactivate Antialising for rectangle drawing
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    //Define pen, brush and rect for the bar
    painter->setPen(Qt::gray);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(QRectF(QPointF(-30.0, -30.0), QPointF(30.0, 30.0)));

    if (!dataIsValid) {
        painter->setPen(Qt::red);
        painter->drawLine(QPointF(-30.0, -30.0), QPointF(30.0, 30.0));
        painter->drawLine(QPointF(-30.0, 30.0), QPointF(30.0, -30.0));
    }

	//Restore the painter with antialising
	painter->restore();

    //Draw Texts around (title, min and max value)
    painter->setPen(Qt::white);
    isPenAlarmColored = false;
    painter->drawText(QRectF(-30, -25, 60, 15), Qt::AlignCenter | Qt::AlignVCenter,titleText);

    if (dataIsValid) {
        painter->drawText(QRectF(-30, -10, 60, 15), Qt::AlignCenter | Qt::AlignVCenter,unitText);
        painter->setPen(Qt::white);

        //Draw readout
        font.setPointSize(14);
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(QRectF(-30, 10, 60, 15), Qt::AlignCenter | Qt::AlignVCenter, QString::number(currentValue, 'f', readoutPrecision));
    }
}

void TextBox::setTitle(QString title)
{
	titleText = title;
}

void TextBox::setUnit(QString unit)
{
	unitText = unit;
}

void TextBox::setBorders(double minimum, double maximum)
{
	minValue = minimum;
	maxValue = maximum;
}

void TextBox::setPrecision(quint8 readout, quint8 bar)
{
	readoutPrecision = readout;
	barPrecision = bar;
}

void TextBox::setValue(double value)
{
    if (value ==-999) {
        dataIsValid = false;
    } else {
        dataIsValid = true;
    }

    if (smooth) {
        currentValue = smoothData.dsp_ema_double(value);
    } else {
        currentValue = value;
    }
	update();
}
