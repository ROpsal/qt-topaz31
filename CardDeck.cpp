///
//    Author: Richard Opsal
// 
//   Purpose: Implementation of CardDeck for game of "31".
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
#include <QObject>


///
//  C / C++ / STL includes.
/// 

#include <algorithm>


///
//  Local includes.
///

#include "Card.h"
#include "CardDeck.h"


///
// Constructor CardDeck() :
///

CardDeck:: CardDeck(QObject *parent)

        : QObject(parent)

    {
    }


///
//  Destructor ~CardDeck() :
/// 

CardDeck:: ~CardDeck()
    {
    }


///
//  Routine CardDeck:: appendCard() :
/// 

void CardDeck:: appendCard(Card * pcard)
    {
    cardList.push_back( pcard ) ;
    pcard->setZValue(cardList.count()) ;
    }


///
//  Routine CardDeck:: removeCard() :
/// 

void CardDeck:: removeCard(Card * pcard)
    {
    // Optimize search by starting at back.
    int indexOf = cardList.lastIndexOf(pcard) ;
    if (-1 != indexOf)
        {
        cardList.removeAt( indexOf ) ;
        }
    }


///
//  Routine CardDeck:: removeTopCard() :
// 
//      Remove the top card from the deck.
/// 

void CardDeck:: removeTopCard()
    {
    if (!cardList.isEmpty())
        cardList.pop_back() ;
    }


///
//  Function CardDeck:: topCard() :
// 
//      Return top card from deck.  NULL is returned on empty.
///

Card * CardDeck:: topCard()
    {
    return (cardList.isEmpty())
        ? NULL 
        : cardList.last() ;
    }


///
//  Routine CardDeck:: clearTheDeck() :
//
//      Remove all cards from this deck.
///

void CardDeck:: clearTheDeck()
    {
    cardList.clear() ;
    }


///
//  Routine CardDeck:: zTheDeck() :
//
//      zOrder the cards in the deck.
///

void CardDeck:: zTheDeck()
    {
    unsigned zvalue=0 ;
    foreach (Card * pcard, cardList)
        pcard->setZValue(++zvalue) ;
    }


///
// Routine CardDeck:: shuffleTheDeck() :
// 
//      Randomly shuffle the cards in the passed deck.
/// 

void CardDeck:: shuffleTheDeck()
    {
    std::random_shuffle( cardList.begin(), cardList.end() ) ;
    }


///
//  Function CardDeck:: scoreTheHand() :
// 
//      Assume three cards in hand plus passed card.
//      What is our best score?
/// 

unsigned CardDeck:: scoreTheHand(Card * pcardIn)
    {
    unsigned ascores [Card::CSuites] = {0} ;
    unsigned acsuites[Card::CSuites] = {0} ;
    unsigned alowcard[Card::CSuites] = {0} ;

    static Scorer scorer ;
    unsigned ccards = (C31HAND < cardList.count()) ? C31HAND : cardList.count() ;
    for (unsigned j=0 ; j<=ccards ; j++) 
        {
        Card * pcard = (j < ccards) ? cardList.at(j) : pcardIn ;
        if (!pcard) break ;

        Card::Suite suite = pcard->getSuite() ;
        unsigned score = scorer( pcard ) ;
        ascores [suite] += score ;
        acsuites[suite] ++ ;

        if ((0 == alowcard[suite]) || (score < alowcard[suite]))
            alowcard[suite] = score ;
        }

    // Locate which suite had highest score.
    unsigned highscore=0 ;
    unsigned highindex=0 ;
    for (unsigned i=0 ; i<Card::CSuites ; i++) 
        if (highscore < ascores[i]) 
            {
            highscore = ascores[i] ;
            highindex = i ;
            }

    // In the rare case we have four cards of the same suite.
    // Need to deduct lowest card value.
    if (C31HAND+1 == acsuites[highindex]) 
        highscore -= alowcard[highindex] ;

    // Return highest score for our hand.
    return highscore ;
    }


///
//  Function CardDeck:: scoreTheHand() :
// 
//      Assume three or four cards in hand.
//      What is our best score with three cards?
/// 

unsigned CardDeck:: scoreTheHand()
    {
    Card * pcard = (C31HAND < cardList.count())
        ? cardList.at(C31HAND)
        : 0 ;

    return scoreTheHand(pcard) ;
    }


///
//  Function CardDeck:: lowestCardInHand() :
// 
//      Assumes C31HAND+1 cards in hand.  Which one should be discarded?
/// 

Card * CardDeck:: lowestCardInHand()
    {
    unsigned ascores [Card::CSuites] = {0} ;
    unsigned acsuites[Card::CSuites] = {0} ;
    unsigned alowcard[Card::CSuites] = {0} ;
    Card *   apcards [Card::CSuites] = {0} ;

    static Scorer scorer ;
    unsigned ccards = (C31HAND+1 < cardList.count()) ? C31HAND+1 : cardList.count() ;
    for (unsigned j=0 ; j<ccards ; j++) 
        {
        Card * pcard = cardList.at(j) ;
        Card::Suite suite = pcard->getSuite() ;
        unsigned score = scorer( pcard ) ;
        ascores [suite] += score ;
        acsuites[suite] ++ ;

        if ((0 == alowcard[suite]) || (score < alowcard[suite]))
            {    
            alowcard[suite] = score ;
            apcards [suite] = pcard ;
            }
        }

    // Which suite has lowest score?
    unsigned lowscore=0 ;
    unsigned lowindex=0 ;
    for (unsigned i=0 ; i<Card::CSuites ; i++) 
        if ((0 == lowscore) || (ascores[i] && (lowscore > ascores[i])))
            {
            lowscore = ascores[i] ;
            lowindex = i ;
            }

    // Return lowest card from suite with lowest sum.
    return apcards[lowindex] ;
    }
