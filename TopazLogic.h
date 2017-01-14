///
//    Author: Richard Opsal
// 
//   Purpose: The logic for playing the 31" game.
//            This is basically the game "controller".
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/13
// $Revision$
//     $Date$
///

#ifndef _TOPAZLOGIC_H
#define _TOPAZLOGIC_H

///
//  Qt includes.
/// 

#include <QObject>
#include <QList>
#include <Players.h>


///
// Classes utilized by this file.
///

class TopazScene ;
class CardDeck ;
class MasterDeck ;


///
//  Class TopazLogic :
///
 
class TopazLogic : public QObject
    {
    Q_OBJECT

    public:
        enum GameState{Inactive, Round_Active, Round_Complete, Game_Complete} ;
        enum TurnAction{Draw, Knock, Thirty_one} ;
    
    public:
        TopazLogic(TopazScene &, Player::PlayLevel=Player::Medium, QObject *pparent = 0) ;
       ~TopazLogic() ;

    private:
        static const unsigned CDEAL_CARDS=3 ;
        void  dealTheCards() ;
        void clearTheCards() ;

    private:
        QList<Player *>             playerList   ; 
        QList<Player *>             playerListOrg;
        unsigned          countOfActivePlayers() ;
        QList<Player *> &     updatePlayerList() ;  // Return playerList ;
        QList<Player *> & initializePlayerList() ;  // Return playerList ;
        QList<Player *> &     rotatePlayerList() ;  // Return playerList ;

    private slots:
        void doDealAction() ;
        void doRestartAction() ;
        void doTurnCompleted( Player *, Player::TurnAction ) ;
        void doGameClose() ;

    private:
        void doTheReckoning() ;

    signals:
        void thirtyOneCheck() ;
        void playerTurn( Player *, bool, unsigned round, unsigned cPlayers, Player::PlayLevel ) ;
        void roundStarting () ;
        void roundCompleted() ;
        void gameStarting (unsigned clives) ;
        void gameCompleted(Player *) ;
    
    private:
        void wirePlayers() ;
        void wireButtons() ;

    private:
        unsigned             cturns ;
        GameState         gameState ;
        Player *       pplayerKnock ;
        Player::PlayLevel playLevel ;

        TopazScene & rtopazScene ;
        MasterDeck & rmasterDeck ;
        CardDeck &   rdealDeck ;
    } ; 

#endif 
