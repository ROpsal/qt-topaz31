///
//    Author: Richard Opsal
// 
//   Purpose: The "view" class into the "31" game scene.
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/13
// $Revision$
//     $Date$
///


#ifndef _TOPAZVIEW_H
#define _TOPAZVIEW_H

///
//  Qt includes.
/// 

#include <QGraphicsView>


///
// Classes utilized by this file.
///
 
class QGraphicsScene ;


///
//  Class TopazView :
/// 

class TopazView : public QGraphicsView
    {
    Q_OBJECT
    
    public:
        TopazView(QGraphicsScene &, QWidget *parent = 0) ;
       ~TopazView() ;

    private:
        void resizeEvent(QResizeEvent* event) ;     
        QSize sizeHint() const ;

    private:
        QSize widgetSize ;
        QGraphicsScene & rgraphicScene ;
    } ;

#endif 
