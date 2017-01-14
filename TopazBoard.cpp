///
//    Author: Richard Opsal
// 
//   Purpose: The entire "31" board.
//            CS3350 demonstration program.
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

#include <QMainWindow>


///
//  Local includes.
///

#include "TopazBoard.h"
#include "TopazScene.h"
#include "TopazLogic.h"
#include "TopazView.h"


///
//  Constructor TopazBoard :
/// 

TopazBoard:: TopazBoard

        (
        Player::PlayLevel playLevelIn,  // IN - Play level.
        quint8             cPlayersIn,  // IN - Count of players.
        QWidget *           pparentIn   // IN - Owner of object.
        )

        : QMainWindow( pparentIn ),
          rtopazScene( * new TopazScene( cPlayersIn ) ),
          rtopazLogic( * new TopazLogic( rtopazScene, playLevelIn ) ),
          rtopazView ( * new TopazView ( rtopazScene ) )

    {
    rtopazView.setScene( &rtopazScene ) ;
    setCentralWidget( &rtopazView ) ;
    }


///
//  Destructor ~TopazBoard :
/// 

TopazBoard:: ~TopazBoard()
    {
    delete &rtopazView ;
    delete &rtopazLogic ;
    delete &rtopazScene ;
    }



