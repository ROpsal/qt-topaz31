///
//    Author: Richard Opsal
// 
//   Purpose: Implementation of Card class.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/13
// $Revision$
//     $Date$
///
 

///
//  Qt includes.
/// 

#include <QGraphicsSvgItem>
#include <QSvgRenderer>


///
//  Local includes.
///

#include "Card.h"
 

///
// Constructor Card() :
/// 
 
Card:: Card

        (
        QSvgRenderer * psvgRendererIn,  // IN - Graphics rendering object.
  const QString &     rstrElementIDIn,  // IN - Name of card. 
        unsigned              valueIn,  // IN - Value of card (points).
        Suite                 suiteIn,  // IN - Suite of card.
        bool              fShowFaceIn   // IN - Is card face visible?
        )

        : QGraphicsSvgItem(),
          psvgRenderer(psvgRendererIn),
          strElementID(rstrElementIDIn),
          value(valueIn),
          suite(suiteIn)

    {
    QGraphicsSvgItem::setSharedRenderer(psvgRenderer) ;
    setCardFaceUp(fShowFaceIn) ;
    }


///
// Destructor ~Card() :
/// 
 
Card:: ~Card()
    {
    }


///
//  Routine Card:: setCardFaceUp() :
// 
//      Front or back of card by setting string into svg table.
/// 

void Card:: setCardFaceUp

        (
        bool    fShowFace           // IN - If 'true', then face of card.
        )

    {
    strActiveID = fShowFace
        ? strElementID
        : "back" ;

    if (strActiveID != QGraphicsSvgItem::elementId())
        QGraphicsSvgItem::setElementId(strActiveID) ;
    }


///
//  Routine Card::mousePressEvent() :
// 
//      Card was selected via a mouse click.
/// 

void Card::mousePressEvent(QGraphicsSceneMouseEvent* pevent)
    {
    emit cardClicked( this ) ;
    }
