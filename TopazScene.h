///
//    Author: Richard Opsal
//
//   Purpose: The "31" scene and its elements.
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/13
// $Revision$
//     $Date$
///


#ifndef _TOPAZSCENE_H
#define _TOPAZSCENE_H

///
//  Qt includes.
///

#include <QGraphicsScene>


///
// Classes utilized by this file.
///

class TopazLogic ;
class Player ;
class NpcPlayer ;
class HmnPlayer ;
class Dealer ;
class Button ;


///
//  Class TopazScene :
//
//      Collection of graphical items representing the game of "31".
//      Game logic controls what is seen and interactions.
///

class TopazScene : public QGraphicsScene
    {
    Q_OBJECT

    public:
        TopazScene(quint8 cPlayersIn, QObject *parent = 0) ;
       ~TopazScene() ;

    public:
        static const unsigned SC_WIDTH  = 1600 ;    // SC = Screen
        static const unsigned SC_HEIGHT = 1000 ;
        static const unsigned DR_WIDTH  =  526 ;    // DR = Dealer Region
        static const unsigned DR_HEIGHT =  280 ;
        static const unsigned CP_WIDTH  =  420 ;    // CP = Computer Player Region
        static const unsigned CP_HEIGHT =  280 ;
        static const unsigned HP_WIDTH  =  900 ;    // HP = Human Player Region
        static const unsigned HP_HEIGHT =  280 ;

    public:
        void showStartButton( bool fShowStart=true ) ;

    private:

        // Count of players.
        unsigned   cPlayers ;

        // Regions on the playing board, aka different roles.
        Dealer    & rdealer ;
        HmnPlayer & rhmnplayer ;
        NpcPlayer * pnpcplayer_1 ;
        NpcPlayer * pnpcplayer_2 ;
        NpcPlayer * pnpcplayer_3 ;

        Button * pbtnDeal ;
        Button * pbtnExit ;
        Button * pbtnKnock ;
        Button * pbtnRestart ;
        Button * pbtnStart ;

        friend class TopazLogic ;

    private:

        enum Position{LEFT, CENTER, RIGHT} ;
        static unsigned cp_xOffset(Position) ;
        static unsigned cp_yOffset(Position) ;

        static unsigned hp_xOffset() ;
        static unsigned hp_yOffset() ;

        static unsigned dr_xOffset() ;
        static unsigned dr_yOffset() ;

        static unsigned btn_xOffset( unsigned width, unsigned height ) ;
        static unsigned db_yOffset (                 unsigned height ) ;
        static unsigned rb_yOffset (                 unsigned height ) ;
        static unsigned eb_yOffset (                 unsigned height ) ;
        static unsigned kb_xOffset ( unsigned width                  ) ;
        static unsigned kb_yOffset (                 unsigned height ) ;

    private:
        void initializePlayers() ;
        void initializeButtons() ;
    } ;

#endif
