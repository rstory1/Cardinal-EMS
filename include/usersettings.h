//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Cardinal EMS, Experimental Aircraft EMS                              //
// Copyright (C) 2017 Ryan Story                                        //
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

#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>

class userSettings : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit userSettings(QGraphicsObject* parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF backgroundRect;
    QRectF oilPbox;
    QRectF oilTbox;
    QRectF oilTcheckbox;
    QRectF manPbox;
    QRectF egtBox;
    QRectF chtBox;
    QRectF ffBox;

signals:

public slots:
    void showSettings(bool tfShow);
};

#endif // USERSETTINGS_H
