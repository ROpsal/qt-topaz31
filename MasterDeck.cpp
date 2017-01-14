///
//    Author: Richard Opsal
// 
//   Purpose: Implementation of MasterDeck for game of "31".
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

#include <QList>
#include <QGraphicsScene>
#include <QSvgRenderer>


///
//  Local includes.
///

#include "Card.h"
#include "CardDeck.h"
#include "MasterDeck.h"


///
// Constructor MasterDeck() :
///

MasterDeck:: MasterDeck(QObject *pparent)

    : QObject(pparent),
      psvgRenderer(new QSvgRenderer(QLatin1String(":/images/svg-cards.svg")))

    {
    createCardList() ;
    }     


///
//  Destructor ~MasterDeck() :
// 
//      Delete allocated card objects.
///

MasterDeck:: ~MasterDeck()
    {
    while (!cardList.isEmpty())
        {
        // List holds pointers to Card objects.
        // Delete card object and remove entry from list.
        Card * pcard = cardList.back() ;
        delete pcard ;
        cardList.pop_back() ;
        }

    // Remove renderer.
    delete psvgRenderer ;
    }


///
//  Routine MasterDeck:: createCardList() :
/// 

void MasterDeck:: createCardList()
    {
    const char * ACPSZ_RANKS [] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "jack", "queen", "king"} ;
    const char * ACPSZ_SUITES[] = {     "spade",     "club",     "diamond",     "heart"} ;
    const Card::Suite ASUITES[] = {Card::Spade, Card::Club, Card::Diamond, Card::Heart} ;
    QString qstr ;
    for (unsigned s = 0 ; s < sizeof(ACPSZ_SUITES) / sizeof(const char *); s++ )
        {
        for (unsigned r = 0 ; r < sizeof(ACPSZ_RANKS) / sizeof(const char *); r++ )
            {
            qstr  = ACPSZ_RANKS[r] ;
            qstr += '_' ;
            qstr += ACPSZ_SUITES[s] ;
            cardList.push_back( new Card(psvgRenderer, qstr, r+1, ASUITES[s] ));
            }
        }
    }


///
//  Routine MasterDeck:: initCardDeck() :
// 
//      Basically, copy cards from our "master" list to target list.
/// 

void MasterDeck:: initCardDeck( CardDeck & rcardDeck )
    {
    rcardDeck.cardList = MasterDeck::cardList ;
    }


///
//  Routine MasterDeck:: removeCardsFromScene() :
// 
//      Removes all the cards from the passed scene.
//      Assumes all cards have placed on the Topaz board.
/// 

void MasterDeck:: removeCardsFromScene(QGraphicsScene & rscene)
    {
    foreach(Card * pcard, cardList)
        rscene.removeItem(pcard) ;
    }
