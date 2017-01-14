///
//    Author: Richard Opsal
// 
//   Purpose: Generic pile of cards for game of "31".
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/13
// $Revision$
//     $Date$
///


#ifndef _CARD_H
#define _CARD_H


///
//  Qt includes.
/// 

#include <QGraphicsSvgItem>


///
//  Classes utilized by this file.
///

class QSvgRenderer ;


///
//  Class Card :
// 
//      A single playing card from a deck of cards.
//      Combines both graphical and logical representations.
/// 

class Card : public QGraphicsSvgItem
    {
    Q_OBJECT

    public:
        enum Suite{Spade, Club, Diamond, Heart, CSuites} ;

    public:
        Card(QSvgRenderer *, const QString & rstrElementID, unsigned valueIn, Suite suiteIn, bool=false) ;
       ~Card() ;

    public:
        void setCardFaceUp(bool fShowFace=true) ;
        bool  isCardFaceUp(){return strElementID == strActiveID ;}

    public:
        unsigned getValue() const {return value;}
        Suite    getSuite() const {return suite;}

    public:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) ;

    signals:
        void cardClicked( Card * ) ;

    private:
        unsigned value ;    // [1..13]
        Suite    suite ;    // The card suite.

        QSvgRenderer * psvgRenderer ;
        QString strElementID ;
        QString strActiveID ;
    } ;


///
// Class Scorer :
// 
//      Translate the passed card value into a "31" score.
//      Implemented as a function object.
/// 

struct Scorer
    {
    unsigned operator()( unsigned value )
        {
        return (1 == value) 
            ? 11
            : (10 < value) ? 10 : value ;
        }
    
    unsigned operator()( Card & rcard )
        {
        return Scorer::operator()( rcard.getValue() ) ;
        }
    
    unsigned operator()( Card * pcard )
        {
        return (pcard) ? Scorer::operator()( *pcard ) : 0 ;
        }
    } ;

#endif
