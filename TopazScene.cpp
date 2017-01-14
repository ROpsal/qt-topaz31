///
//    Author: Richard Opsal
// 
//   Purpose: Implementation of the "31" scene and its elements.
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

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QList>


///
// Local includes.
///

#include "TopazScene.h"
#include "Players.h"
#include "Button.h"


///
// Routines for calculating region x, y offsets for actors.
///

unsigned TopazScene:: cp_xOffset(TopazScene::Position pos)
    {
    unsigned delta = SC_WIDTH - 3*CP_WIDTH ;
    delta /= 3 ;
    unsigned xOffset = delta>>1 ;

    if (TopazScene::CENTER == pos)
        xOffset += delta + CP_WIDTH ;

    if (TopazScene::RIGHT == pos)
        xOffset += (delta + CP_WIDTH) << 1 ;

    return xOffset ;
    }

unsigned TopazScene:: cp_yOffset(TopazScene::Position pos)
    {
    unsigned delta = SC_HEIGHT - DR_HEIGHT - CP_HEIGHT - HP_HEIGHT ;
    delta >>= 2 ;
    return (TopazScene::CENTER == pos) ? delta : 4*delta ;
    }

unsigned TopazScene:: hp_xOffset()
    {
    return (SC_WIDTH - HP_WIDTH) >> 1 ;
    }

unsigned TopazScene:: hp_yOffset()
    {
    unsigned delta = SC_HEIGHT - DR_HEIGHT - CP_HEIGHT - HP_HEIGHT ;
    delta >>= 2 ;
    return CP_HEIGHT + DR_HEIGHT + 3*delta ;
    }

unsigned TopazScene:: dr_xOffset()
    {
    return (SC_WIDTH - DR_WIDTH) >> 1 ;
    }

unsigned TopazScene:: dr_yOffset()
    {
    unsigned delta = SC_HEIGHT - DR_HEIGHT - CP_HEIGHT - HP_HEIGHT ;
    delta >>= 2 ;
    return CP_HEIGHT + 2*delta ;
    }


///
//  Routines for calculating x,y offsets for buttons.
///

unsigned TopazScene:: btn_xOffset( unsigned width, unsigned height )
    {
    return SC_WIDTH - width - (height>>1) ;
    }

unsigned TopazScene:: db_yOffset( unsigned height )
    {
    return SC_HEIGHT - (height<<1) ;
    }

unsigned TopazScene:: eb_yOffset( unsigned height )
    {
    return height>>1 ;
    }

unsigned TopazScene:: rb_yOffset( unsigned height )
    {
    return height<<1 ;
    }

unsigned TopazScene:: kb_xOffset( unsigned width )
    {
    return hp_xOffset() + HP_WIDTH - width ;
    }

unsigned TopazScene:: kb_yOffset( unsigned height )
    {
    return SC_HEIGHT - height*8 ;
    }


///
//  Constructor TopazScene :
///

TopazScene:: TopazScene

        (
        quint8   cPlayersIn,        // IN - Number of game players.
        QObject * pparentIn         // IN - Owner of this object.
        )

    : QGraphicsScene(QRectF(0, 0, SC_WIDTH, SC_HEIGHT), pparentIn),
      cPlayers(cPlayersIn),
      rdealer   ( * new Dealer    ),
      rhmnplayer( * new HmnPlayer(rdealer, *this) )

    {
    initializePlayers() ;
    initializeButtons() ;
    }


///
//  Destructor ~TopazScene() :
/// 

TopazScene:: ~TopazScene()
    {
    delete &rdealer ;
    delete &rhmnplayer ;

    delete pnpcplayer_1 ;
    delete pnpcplayer_2 ;
    delete pnpcplayer_3 ;
    }


///
//  Routine TopazScene:: initializePlayers() :
// 
//      Create and initialize players.
/// 

void TopazScene:: initializePlayers()
    {
    // Position the dealer in the middle.
    rdealer.setRect(dr_xOffset(), dr_yOffset(), DR_WIDTH, DR_HEIGHT) ;
    QGraphicsScene::addItem( &rdealer ) ;

    // Human player towards bottom.
    rhmnplayer.setRect(hp_xOffset(), hp_yOffset(), HP_WIDTH, HP_HEIGHT) ;
    QGraphicsScene::addItem( &rhmnplayer ) ;

    // NPC players from left to right, on top.
    pnpcplayer_1 = new NpcPlayer(rdealer, *this) ;
    pnpcplayer_2 = new NpcPlayer(rdealer, *this) ;
    pnpcplayer_3 = new NpcPlayer(rdealer, *this) ;

    pnpcplayer_1->setRect(cp_xOffset(LEFT), cp_yOffset(LEFT), CP_WIDTH, CP_HEIGHT) ;
    QGraphicsScene::addItem( pnpcplayer_1 ) ;

    pnpcplayer_2->setRect(cp_xOffset(CENTER), cp_yOffset(CENTER), CP_WIDTH, CP_HEIGHT) ;
    QGraphicsScene::addItem( pnpcplayer_2 ) ;

    pnpcplayer_3->setRect(cp_xOffset(RIGHT), cp_yOffset(RIGHT), CP_WIDTH, CP_HEIGHT) ;
    QGraphicsScene::addItem( pnpcplayer_3 ) ;
    }


///
//  Routine TopazScene:: initializeButtons() :
// 
//      Create and initialize buttons.
/// 

void TopazScene:: initializeButtons()
    {
    QList<Button **> listButtons ;
    listButtons << &pbtnDeal << &pbtnExit << &pbtnKnock << &pbtnRestart << &pbtnStart ;

    static const char * ACPSZIMAGES[] = 
        {
        ":/images/deal_button.png",
        ":/images/exit_button.png",   
        ":/images/knock_button.png",  
        ":/images/restart_button.png",
        ":/images/start_button.png"  
        } ;

    unsigned index = 0 ;
    const qreal zscale = 0.8 ;
    foreach (Button ** ppbtn, listButtons)
        {
         *ppbtn = new Button(QPixmap(ACPSZIMAGES[index++])) ;
        (*ppbtn) -> setScale( zscale ) ;
        }

    // Place the buttons.
    QRectF rect =	pbtnDeal-> boundingRect () ;
    unsigned width = rect.width () * zscale ;
    unsigned height= rect.height() * zscale ;
    pbtnDeal    -> setPos(btn_xOffset( width, height ), db_yOffset( height )) ;
    pbtnExit    -> setPos(btn_xOffset( width, height ), eb_yOffset( height )) ;
    pbtnKnock   -> setPos( kb_xOffset( width         ), kb_yOffset( height )) ;
    pbtnRestart -> setPos(btn_xOffset( width, height ), rb_yOffset( height )) ;
    pbtnStart   -> setPos(btn_xOffset( width, height ), db_yOffset( height )) ;

    // Add buttons to scene.
    foreach (Button ** ppbtn, listButtons)
        QGraphicsScene::addItem( *ppbtn ) ;

    // Show "Start" instead of "Deal" button.
    TopazScene::showStartButton() ;
    }


///
//  Routine TopazScene:: showStartButton() :
// 
//      Show "Start" or "Deal" button.
/// 

void TopazScene:: showStartButton( bool fShowStart )
    {
    pbtnDeal   ->setVisible( !fShowStart ) ;
    pbtnStart  ->setVisible(  fShowStart ) ;
    pbtnRestart->setVisible( !fShowStart ) ;
    }
