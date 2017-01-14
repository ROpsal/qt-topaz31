///
//    Author: Richard Opsal
// 
//   Purpose: Master card deck for game of "31".
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/13
// $Revision$
//     $Date$
///


#ifndef _MASTERDECK_H
#define _MASTERDECK_H


///
//  Qt includes.
/// 

#include <QList>
#include <QObject>
#include <QSvgRenderer>


///
// Classes utilized by this file.
///
 
class Card ;
class CardDeck ;
class QGraphicsScene ;


///
// Class MasterDeck :
// 
//      This container loads playing card svg images from the
//      application resource file; creating Card objects.
// 
//      CardDeck's utilize these Card objects for graphical rendering
//      of cards.
// 
//      The MasterDeck is responsible for the deletion of Card objects.
///

class MasterDeck : public QObject
    {
    public:
        MasterDeck(QObject *pparent = 0) ;
       ~MasterDeck() ;

    public:
        void initCardDeck( CardDeck & ) ;
        void removeCardsFromScene(QGraphicsScene &) ;

    private:
        void createCardList() ;

        QList<Card *>      cardList ;
        QSvgRenderer * psvgRenderer ;
    } ;

#endif
