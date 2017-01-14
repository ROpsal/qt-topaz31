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


#ifndef _TOPAZBOARD_H
#define _TOPAZBOARD_H

///
//  Qt includes.
/// 

//#include <QWidget>
#include <QMainWindow>


///
//  Local includes.
///

#include "TopazLogic.h"
#include "Players.h"


///
// Classes utilized by this file.
///
 
//class QWidget ;
class TopazView ;
class TopazScene ;
class TopazLogic ;


///
//  Class TopazBoard :
/// 

class TopazBoard : public QMainWindow
    {
    Q_OBJECT
    
    public:
        TopazBoard(Player::PlayLevel=Player::Medium, quint8 cPlayers=4, QWidget *parent = 0) ;
       ~TopazBoard() ;
    
    private:
        TopazScene & rtopazScene ;
        TopazLogic & rtopazLogic ;
        TopazView &  rtopazView ;
    } ;

#endif
