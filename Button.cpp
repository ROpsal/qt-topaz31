///
//    Author: Richard Opsal
// 
//   Purpose: Implementation of action button for game of "31".
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/19
// $Revision$
//     $Date$
///


///
//  Qt includes.
/// 

#include <QObject>
#include <QGraphicsPixmapItem>


///
//  Local includes.
///

#include "Button.h"


///
//  Constructor Button :
///

Button:: Button(QObject *pparent)

        : QObject( pparent )

    {
    setAcceptedMouseButtons(Qt::LeftButton) ;
    }


Button:: Button(const QPixmap & ppixmap, QObject * pparent)

        : QObject( pparent ),
          QGraphicsPixmapItem( ppixmap )

    {    
    setAcceptedMouseButtons(Qt::LeftButton) ;
    }


///
//  Destructor ~Button :
/// 

Button:: ~Button()
    {
    }


///
//  Routine Button:: mousePressEvent() :
// 
//      Translate passed event into a regular Qt signal.
//
//      Apparently, for mouseReleaseEvent(), item must be movable or 
//      selectable.
///

void Button:: mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
    emit buttonClicked() ;
    }
