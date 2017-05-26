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

#ifndef WINDVECTOR_H
#define WINDVECTOR_H

#include <QtWidgets>

class WindVector : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit WindVector(QGraphicsObject * parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void updateWind(float spd, float dir, float mHdg);

private:
    QPolygonF vector;
    QPolygonF vector2;
    QTransform t;
    float windSpd = 0.0;
    float windDir = 0.0; // This is the compass direction the wind is from
    float magHdg = 0.0;
    float windX = 0.0;
    float windY = 0.0;
    QRectF spdTextRect;
    float PI = 3.145;

};

#endif // WINDVECTOR_H
