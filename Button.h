///
//    Author: Richard Opsal
// 
//   Purpose: Action button for game of "31".
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/19
// $Revision$
//     $Date$
///


#ifndef _BUTTON_H
#define _BUTTON_H


///
//  Qt includes.
/// 

#include <QObject>
#include <QGraphicsPixmapItem>


///
//  Classes utilized by this file.
///

class QPixmap ;


///
//  Class Button :
// 
//      Combines the pixmap item with a QObject allowing the
//      buttonClicked() signal.
/// 

class Button : public QObject, public QGraphicsPixmapItem
    {
    Q_OBJECT

    public:
        Button(QObject *pparent=0) ;
        Button( const QPixmap & ppixmap, QObject * pparent = 0 ) ;
       ~Button() ;

    public:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) ;

    signals:
        void buttonClicked() ;
    } ;

#endif
