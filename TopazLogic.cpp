///
//    Author: Richard Opsal
// 
//   Purpose: Implementation of the logic for playing the 31" game.
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/16
// $Revision$
//     $Date$
///

///
//  Qt includes.
/// 

#include <QCoreApplication>
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>


///
//  Local includes.
///

#include "TopazScene.h"
#include "TopazLogic.h"
#include "CardDeck.h"
#include "MasterDeck.h"
#include "Players.h"
#include "Button.h"


///
//  Constructor TopazLogic :
///
 
TopazLogic:: TopazLogic

        (
        TopazScene &    rtopazSceneIn, // IN - Scene representing game.
        Player::PlayLevel playLevelIn,   // IN - Difficult of play level.
        QObject *           pparentIn      // IN - Owner of object.
        )

        : QObject( pparentIn ),
          playLevel  (      playLevelIn ),
          gameState  (       Inactive   ), 
          rtopazScene(    rtopazSceneIn ),
          rmasterDeck( * new MasterDeck ),
          rdealDeck  ( * new   CardDeck )

    {
    // Generate a list of active players.
    initializePlayerList() ;

    // Wire players and buttons, i.e., signal and slot connections.
    wirePlayers() ;
    wireButtons() ;

    // Deal some cards so initial board display isn't empty.
    // Configure to look like end of game.
    emit gameStarting( Player::CLIVES ) ;
    emit roundStarting () ;
    TopazLogic:: dealTheCards() ;
    emit roundCompleted() ;
    emit gameCompleted(0) ;
    }


///
//  Destructor TopazLogic :
///
 
TopazLogic:: ~TopazLogic()
    {
    // This allows the "master" deck to cleanly delete the cards.
    rmasterDeck.removeCardsFromScene( rtopazScene ) ;

    // Get rid of the individual card decks.
    delete &rdealDeck ;
    delete &rmasterDeck ;
    }


///
//  Routine TopazLogic:: dealTheCards() :
// 
//      Deal of the cards at start of round.
// 
//  Assumption:
// 
//      All cards removed from scene prior to deal.
///

void TopazLogic:: dealTheCards()
    {
    // Get a new deck of cards and shuffle them.
    rmasterDeck.initCardDeck( rdealDeck ) ;
    rdealDeck.shuffleTheDeck() ;

    // Get list of active players.
    QList<Player *> & rplayerList = TopazLogic:: updatePlayerList() ;

    // Here we deal cards to the players.  We even do this in turn!
    unsigned cCards=CDEAL_CARDS ;
    while (cCards--)
        foreach(Player * pplayerRegion, rplayerList)
            {
            Card * ptopCard = rdealDeck.topCard() ;
            pplayerRegion->appendCard( ptopCard ) ;
            rdealDeck.removeTopCard() ;
            }

    // One card goes to the discard pile.
    Card * ptopCard = rdealDeck.topCard() ;
    rtopazScene.rdealer.appendDiscard( ptopCard ) ;
    rdealDeck.removeTopCard() ;

    // Remainder of cards go to the draw deck.
    while (!rdealDeck.isEmpty())
        {
        Card * ptopCard = rdealDeck.topCard() ;
        rtopazScene.rdealer.appendDrawCard( ptopCard ) ;
        rdealDeck.removeTopCard() ;
        }
    }


///
//  Function TopazLogic:: countOfActivePlayers() :
/// 

unsigned TopazLogic:: countOfActivePlayers()
    {
    unsigned count = 0 ;
    foreach(Player * pplayer, playerList)
        {
        if (pplayer->isPlayerActive())
            count++ ;
        }

    return count ;
    }


///
//  Function TopazLogic:: updatePlayerList() :
// 
//      Players with no more tokens or not on-your-honour are removed.
///

QList<Player *> & TopazLogic:: updatePlayerList()
    {
    foreach(Player * pplayer, playerListOrg)
        {
        if (!pplayer->isPlayerActive())
            playerList.removeAll( pplayer ) ;
        }

    return playerList ;
    }


///
//  Function TopazLogic:: initializePlayerList() :
// 
//      List of players at start of game.
///

QList<Player *> & TopazLogic:: initializePlayerList()
    {
    playerList.clear() ;
    unsigned cPlayers = rtopazScene.cPlayers ;

    if (cPlayers--)
        playerList.push_back( & rtopazScene.rhmnplayer ) ;

    if (cPlayers--)
        playerList.push_back( rtopazScene.pnpcplayer_1 ) ;

    if (cPlayers--)
        playerList.push_back( rtopazScene.pnpcplayer_2 ) ;

    if (cPlayers--)
        playerList.push_back( rtopazScene.pnpcplayer_3 ) ;

    // Save copy of original player list.
    playerListOrg.clear() ;
    playerListOrg = playerList ;

    // Randomly select who goes first.
    unsigned rotates = 1 + rand() % playerList.count() ;
    while (--rotates) rotatePlayerList() ;

    return playerList ;
    }


///
//  Function TopazLogic:: rotatePlayerList() :
// 
//      Rotate player order.  
//      Make sure only active players present.
//
//      This routine may be slightly off as players are removed. 
///

QList<Player *> & TopazLogic:: rotatePlayerList()
    {
    cturns = 0 ;
    pplayerKnock = 0 ;
    if ( 1 < playerList.count() )
        {
        playerList.push_back(playerList.front()) ;
        playerList.pop_front() ;
        }

    return updatePlayerList() ;
    }


///
//  Slot Routine TopazLogic:: doDealAction() :
///

void TopazLogic:: doDealAction()
    {
    switch (gameState)
        {
        case Inactive:
        case Game_Complete:
            // Actions at start of game.  This resets tokens
            emit gameStarting( Player::CLIVES ) ;

            // Regenerate active player list.  Dependent on tokens!
            initializePlayerList() ;

            // New round beginning.
            emit roundStarting() ;
            gameState = Round_Active ;

            // Need fresh deal of cards.
            dealTheCards() ;

            // Just in case someone was dealt 31.
            emit thirtyOneCheck() ;

            // This triggers start of game play to first player in list.
            emit playerTurn( playerList[0], false, 1, playerList.count(), playLevel ) ;

            // Show the "Deal" button instead of "Start" button.
            rtopazScene.showStartButton(false) ;
            break ;

        case Round_Active:
            // No deal in the middle of a round.
            break ;

        case Round_Complete:
            // This rotates dealer to next position.
            rotatePlayerList() ;

            // New round beginning.
            emit roundStarting() ;
            gameState = Round_Active ;

            // Need fresh deal of cards.
            dealTheCards() ;

            // This triggers start of game play to first player in list.
            emit playerTurn( playerList[0], false, 1, playerList.count(), playLevel ) ;
            break ;
        }

    }


///
//  Slot Routine TopazLogic:: doRestartAction() :
// 
//      Restart the game.
/// 

void TopazLogic:: doRestartAction()
    {
    gameState = Round_Complete ;
    emit roundCompleted() ;

    gameState = Game_Complete ;
    emit gameCompleted(0) ; 

    // Do a slight delay before start of new game.
    QTimer::singleShot(500, this, SLOT(doDealAction())) ;
    }


///
//  Slot Routine TopazLogic:: doTurnCompleted() :
///

void TopazLogic:: doTurnCompleted

        ( 
        Player *              pplayerIn,    // IN - Which player.
        Player::TurnAction turnActionIn     // IN - What action was taken.
        )

    {
    // Do this check in case of game reset.
    if (Game_Complete == gameState)
        return ;

    // Get current and next player.
    Player * pplayerCurrent = playerList[   cturns % playerList.count() ] ;
    Player * pplayerNext    = playerList[ ++cturns % playerList.count() ] ;

    // Should check that pplayerIn matches pplayerCurrent.
    // This could happen when a player has "31".
    if (  (pplayerCurrent != pplayerIn)
        &&(turnActionIn   != Player::Thirty_one))
        {
        // Problem if here!
        }

    // Have we finished a round?  Or have "31"?
    if (  (pplayerKnock == pplayerNext)
        ||((0 == pplayerKnock) && (turnActionIn == Player::Thirty_one)))
        {
        // Do the reckoning.
        doTheReckoning() ;

        // Completed this round.
        gameState = Round_Complete ;

        // Show other players' cards.
        emit roundCompleted() ;

        // Check if this game has completed.
        if (1 == countOfActivePlayers())
            {
            // Round just completed has more than one active player.
            // Need to adjust to only the winner.
            updatePlayerList() ;

            // Signal players that game has completed.
            emit gameCompleted(playerList[0]) ; 

            // Change our game state.
            gameState = Game_Complete ;

            // Change from "Deal" to "Start" button on board.
            rtopazScene.showStartButton(true) ;
            }
        }

    else
        {
        // Do we have a knock action ?
        if (turnActionIn == Player::Knock)
            pplayerKnock = pplayerIn ;

        // Which round are we on.
        unsigned round = 1 + cturns / playerList.count() ;
    
        // Have next player take turn.
        emit playerTurn( pplayerNext, (0 != pplayerKnock), round, playerList.count(), playLevel ) ;
        }
    }


///
//  Slot Routine TopazLogic:: doGameClose() :
/// 

void TopazLogic:: doGameClose()
    {
    ///
    //  Add dialog if game is in progress.  Verify that we really should
    //  exit.
    /// 

    QCoreApplication::instance () -> quit() ;
    }


///
//  Slot Routine TopazLogic:: doTheReckoning() :
// 
//      One or more players has thirty-one.
//    Or
//      A player knocked and ended the round.
/// 

void TopazLogic:: doTheReckoning() 
    {
    // Locate the low score.
    unsigned lowscore = 0 ;
    foreach(Player * pplayer, playerList)
        {
        unsigned playerscore = pplayer->scoreThePlayer() ;
        if ((0 == lowscore) || (lowscore > playerscore))
            lowscore = playerscore ;
        }

    // If a player knocked but had the lowest score, lose two lives.
    if ((pplayerKnock) && (lowscore == pplayerKnock->scoreThePlayer()))
        {
        pplayerKnock -> removeAToken() ;
        pplayerKnock -> removeAToken() ;
        }

    // If a knock round, then players with same lowest score lose one life.
    else if (pplayerKnock)
        {
        foreach(Player * pplayer, playerList)
            if (lowscore == pplayer->scoreThePlayer())
                pplayer -> removeAToken() ;
        }

    // Everybody without "31" score lose a life.
    else
        foreach(Player * pplayer, playerList)
            if (CardDeck::C31SUM != pplayer->scoreThePlayer())
                pplayer -> removeAToken() ;
    }


///
//  Routine TopazLogic:: wirePlayers() :
// 
//      Signal / slot wiring with players.
///

void TopazLogic:: wirePlayers()
    {
    // Signals for start of new round.  Special connect for dealer object.
    connect( this, SIGNAL(roundStarting()), &rtopazScene.rdealer, SLOT(doRoundStarting()) ) ;

    // Initialize signals and slots for each player.
    foreach(Player * pplayer, playerListOrg)
        {
        // Signal for start of new round.
        connect( this, SIGNAL(roundStarting()), pplayer, SLOT(doRoundStarting()) ) ;

        // Signal for end-of-round.
        connect( this, SIGNAL(roundCompleted()), pplayer, SLOT(doRoundCompleted()) ) ;

        // Signal for start of new game.
        connect( this, SIGNAL(gameStarting(unsigned)), pplayer, SLOT(doGameStarting(unsigned)) ) ;

        // Signal for end-of-game.
        connect( this, SIGNAL(gameCompleted(Player *)), pplayer, SLOT(doGameCompleted(Player *)) ) ;

        // At beginning of game, do a check for 31.
        connect( this,    SIGNAL(thirtyOneCheck()),
                 pplayer, SLOT(doThirtyOneCheck() ) ) ;

        // Signal for processing player turns.
        connect( this,    SIGNAL(playerTurn(Player *, bool, unsigned, unsigned, Player::PlayLevel)), 
                 pplayer, SLOT(doPlayerTurn(Player *, bool, unsigned, unsigned, Player::PlayLevel)) ) ;

        // Signal for processing player turns.
        connect( pplayer, SIGNAL(turnCompleted(Player *, Player::TurnAction)), this, SLOT(doTurnCompleted(Player *, Player::TurnAction)) ) ;
        }
    }


///
//  Routine TopazLogic:: wireButtons() :
// 
//      Signal / slot wiring with players.
///

void TopazLogic:: wireButtons()
    {
    // Processing of the "Deal" button.
    connect(rtopazScene.pbtnDeal , SIGNAL(buttonClicked()), this, SLOT( doDealAction())) ;
    connect(rtopazScene.pbtnStart, SIGNAL(buttonClicked()), this, SLOT( doDealAction())) ;

    // Processing of the "Restart" button.
    connect(rtopazScene.pbtnRestart, SIGNAL(buttonClicked()), this, SLOT( doRestartAction())) ;

    // Processing of the "Knock" button.
    connect( rtopazScene.pbtnKnock, SIGNAL(buttonClicked()), &rtopazScene.rhmnplayer, SLOT(doKnockAction()) ) ;

    // Processing of the "Exit" button.
    connect(rtopazScene.pbtnExit, SIGNAL(buttonClicked()), this, SLOT(doGameClose())) ;
    }
