///
//    Author: Richard Opsal
// 
//   Purpose: Implementation of the "view" class into the "31" scene.
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

#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QOpenGLWidget>


///
// Classes utilized by this file.
///
 
#include "TopazView.h"
#include "TopazScene.h"


///
//  Constructor TopazView :
///

TopazView:: TopazView

        (
        QGraphicsScene & rgraphicSceneIn, // IN - Scene we're viewing.
        QWidget *        pparentIn        // IN - Owner of this object.
        )

        : QGraphicsView( pparentIn ),
          rgraphicScene( rgraphicSceneIn ),
          widgetSize(TopazScene::SC_WIDTH*0.6, TopazScene::SC_HEIGHT*0.6) 

    {
    // Nice green for card playing table.
    QGraphicsView::setBackgroundBrush(QBrush(QColor("#388E3C"))) ;  // Green 700

    // No scroll bars.
    QGraphicsView:: setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff) ;
    QGraphicsView:: setVerticalScrollBarPolicy  (Qt::ScrollBarAlwaysOff) ;

    // Make a better guess for suggested viewing port size.
    QRect rctDesktop( QApplication:: desktop() -> availableGeometry() ) ;
    widgetSize.setWidth (rctDesktop.width () * 0.8) ;
    widgetSize.setHeight(rctDesktop.height() * 0.8) ;

    // Hopefully an optimization with OpenGL.
    QOpenGLWidget * popenGlWidget = new QOpenGLWidget ;
    setViewport(popenGlWidget);
    }


///
//  Destructor TopazView :
/// 

TopazView:: ~TopazView()
    {
    }


///
//  Routine TopazView::sizeHint() :
/// 

QSize TopazView::sizeHint() const
    {
    // Recommend size for widget.
    return widgetSize ;
    }


///
// Routine TopazView:: resizeEvent() :
//
//    Scale the scene as appropriate.
///

void TopazView:: resizeEvent(QResizeEvent * pevent)
    {
    // Save this for later.
    widgetSize = pevent->size() ;
    
    // Fits the scene inside the current view.
    TopazView::fitInView( rgraphicScene.sceneRect(), Qt::KeepAspectRatio ) ;
    
    // Pass event up the ancestor tree.
    QWidget::resizeEvent(pevent);
    }


