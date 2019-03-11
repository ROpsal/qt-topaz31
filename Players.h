///
//    Author: Richard Opsal
// 
//   Purpose: Player types for the "31" game.
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/13
// $Revision$
//     $Date$
///


#ifndef _REGIONS_H
#define _REGIONS_H


///
//  Qt includes.
/// 

#include <QBrush>
#include <QGraphicsRectItem>
#include <QList>
#include <QObject>


///
//  Local includes.
///

#include "CardDeck.h"


///
// Classes utilized by this file.
///
 
class Card ;
class QGraphicsScene ;
class QGraphicsPixmapItem ;
class Dealer ;


///
// Class Player :
// 
//      Abstract player for "31" card game.
//      Representation encompasses both graphical area plus player logic.
/// 

class Player : public QObject, public QGraphicsRectItem
    {
    Q_OBJECT
    
    public:
        enum PlayLevel{Easy, Medium, Difficult} ;
        enum TurnAction{Draw, Knock, Thirty_one} ;

    public:
        Player(Dealer & rdealer, QGraphicsScene & rscene, QGraphicsItem * pparent = 0 ) ;
        virtual ~Player() ;

    public:
        virtual void appendCard(Card * pcard) ;
        virtual void removeCard(Card * pcard) ;
        unsigned scoreThePlayer(){return rcdeck.scoreTheHand() ;}

    public:
        static const unsigned CLIVES=4 ;
        void initializeTokens(unsigned=CLIVES) ;
        void     removeAToken() ;
        bool   isPlayerActive() ;

    public slots:
                void doThirtyOneCheck() ;
        virtual void doPlayerTurn( Player *, bool, unsigned round, unsigned cPlayers, Player::PlayLevel ) = 0;
        virtual void doRoundStarting () = 0 ;
        virtual void doRoundCompleted() = 0 ;
        virtual void doGameStarting  (unsigned clives) = 0 ;
        virtual void doGameCompleted (Player *) = 0 ;

    signals:
        void turnCompleted( Player *, Player::TurnAction ) ;

    protected:
        // This doesn't remove cards from the TopazScene object!
        void clearTheDeck(){rcdeck.clearTheDeck() ;}

        // Helper functions for display.
        void showPlayerActive ( bool fShowActive = true ) ;
        void showPlayerKnocked( bool fShowKnocked= true ) ;
        void showPlayerNormal (){showPlayerActive(false);showPlayerKnocked(false);}
        void showPlayerLost() ;
        void showPlayerWon() ;

    public:
        // Case when player has CardDeck::C31SUM score.
        void showPlayerHas31() ;

    protected:
        // Get cards from here.
        QGraphicsScene & rscene ;
        
        // Lives or "tokens" stored in this container.
        QList<QGraphicsPixmapItem *> listTokens ;

        // When out of lives (tokens), still have "on-your-honour" life.
        bool fOnYourHonour ;

        // Our card "hand" is stored here.
        CardDeck & rcdeck ;
        
        // Obtain cards from the dealer.
        Dealer & rdealer ;

    private:
        // Coloring for showing active and "knocked" players.
        QBrush  brushActive ;
        QBrush  brushKnock ;
        QBrush  brushKnockHL ;
        QBrush  brushLostHL ;
        QBrush  brushLostOL ;
        QBrush  brushWinnerHL ;
        QBrush  brushWinnerOL ;
        QBrush  brushHas31HL ;
        QBrush  brushHas31OL ;
        QBrush  brushNone ;
        QBrush  brushOutline ;
        static const unsigned cpelsKnock  = 12 ;
        static const unsigned cpelsLost   =  8 ;
        static const unsigned cpelsNormal =  4 ;
    } ;


///
// Class NpcPlayer :
// 
//      Representation of the non-player character, i.e., the computer.
/// 

class NpcPlayer : public Player
    {
    Q_OBJECT
    
    public:
        NpcPlayer(Dealer & rdealer, QGraphicsScene & rscene, QGraphicsItem * parent = 0 ) ;
       ~NpcPlayer() ;

    public:
        void appendCard(Card * pcard) ;
        void removeCard(Card * pcard) ;
        void setCardsFaceUp(bool fShowFace=true) ;

    public slots:
        void doPlayerTurn( Player *, bool, unsigned round, unsigned cPlayers, Player::PlayLevel ) ;
        void doRoundStarting () ;
        void doRoundCompleted() ;
        void doGameStarting  (unsigned clives) ;
        void doGameCompleted (Player *) ;

    private slots:
        void doPlayerDelay() ;

    private:
        TurnAction turnaction ;
    } ;


///
// Class HmnPlayer :
// 
//      Representation of the human player.
/// 

class HmnPlayer : public Player
    {
    Q_OBJECT
    
    public:
        HmnPlayer( Dealer & rdealer, QGraphicsScene & rscene, QGraphicsItem * parent = 0 ) ;
       ~HmnPlayer() ;

    public:
        void appendCard(Card * pcard) ;
        void removeCard(Card * pcard) ;

    public slots:
        void doPlayerTurn( Player *, bool, unsigned round, unsigned cPlayers, Player::PlayLevel ) ;
        void doRoundStarting () ;
        void doRoundCompleted() ;
        void doGameStarting  (unsigned clives) ;
        void doGameCompleted (Player *) ;

    private:
        void   wireMyCards() ;
        void unwireMyCards() ;
        void   wireDealer () ;
        void unwireDealer () ;

    private slots:
        void doKnockAction() ;
        void doDrawFromDiscard() ;
        void doDrawFromDeck() ;
        void doDiscardFromHand(Card *) ;

    private:
        bool fMyTurn ;
        bool fKnockRound ;
    } ;


///
// Class Dealer :
// 
//      Abstraction of the card dealer.
//      Represents the graphical area of the game plus logic.
/// 

class Dealer : public QObject, public QGraphicsRectItem
    {
    Q_OBJECT
    
    public:
        Dealer(QGraphicsItem * parent = 0 ) ;
       ~Dealer() ;

    public:
        Card *  topDrawCard() ;
        void appendDrawCard(Card * pcard) ;
        void removeDrawCard(Card * pcard) ;

    public:
        Card *  topDiscard() {return rcdeckDiscard.topCard() ;}
        void appendDiscard(Card * pcard) ;
        void removeDiscard(Card * pcard) ;

    public slots:
        void doRoundStarting() ;

    private:
        void clearTheDecks(){rcdeckDraw.clearTheDeck();rcdeckDiscard.clearTheDeck();}

    private:
        static const unsigned cpelsNormal = 4 ;
        QBrush  brushFill ;
        QBrush  brushOutline ;

        CardDeck & rcdeckDraw ;
        CardDeck & rcdeckDiscard ;
    } ;

#endif
