///
//    Author: Richard Opsal
// 
//   Purpose: Generic card deck for game of "31".
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/15
// $Revision$
//     $Date$
///

#ifndef _CARDDECK_H
#define _CARDDECK_H

///
//  Qt includes.
/// 

#include <QObject>
#include <QList>


///
// Classes utilized by this file.
///
 
class Card ;
class MasterDeck ;
class NpcPlayer ;
class HmnPlayer ;
class Dealer ;


///
//  Class CardDeck :
// 
//      Generic "pile" of cards.  
//      The CardDeck object does not "own" contained cards.
//      The MasterDeck object is the card owner.
/// 

class CardDeck : public QObject
    {
    public:
        CardDeck(QObject *parent = 0) ;
       ~CardDeck() ;
    
    public:
        void appendCard(Card * pcard) ;
        void removeCard(Card * pcard) ;
        void removeTopCard() ;

    public:
        unsigned count(){return cardList.count  () ;}
        bool   isEmpty(){return cardList.isEmpty() ;}
        Card * topCard() ;

    public:
        void   clearTheDeck() ;
        void       zTheDeck() ;
        void shuffleTheDeck() ;

    public:
        static const unsigned C31HAND=3 ;   // Three cards in a hand.
        static const unsigned C31SUM=31 ;   // Try to reach "31" for score.
        unsigned   scoreTheHand(Card *) ;
        unsigned   scoreTheHand() ;
        Card * lowestCardInHand() ;

    private:
        friend class MasterDeck ;
        friend class NpcPlayer ;
        friend class HmnPlayer ;
        friend class Dealer ;
        QList<Card*> cardList ;
    } ; 
    
#endif
