///
//    Author: Richard Opsal
// 
//   Purpose: Application main() file.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/13
// $Revision$
//     $Date$
///


///
//  C / C++ / STL includes.
/// 

#include <cstdlib>
#include <ctime>


///
//  Qt includes.
/// 

#include <QApplication>
#include <QDesktopWidget>
//#include <QDebug>


///
//  Local includes.
///

#include "TopazBoard.h"


///
//  Main Routine :
//
//      Launch the Topaz31 project main window.
///

int main( int argc, char **argv )
    {
    // Seed the random number generator.
    srand( unsigned ( time(NULL) ) ) ;

    QApplication app( argc, argv ) ;

    /* Locating the plugins folder.
    QString plugpath = QCoreApplication::applicationDirPath () + "/plugins" ;
    qDebug() << plugpath ;
    QCoreApplication::addLibraryPath(plugpath) ;

    foreach (QString path, app.libraryPaths())
        qDebug() << path ;
    */

    // This is our main window setup.
    TopazBoard topazBoard ;
    app.setActiveWindow( &topazBoard ) ;
    topazBoard.show() ;

    // Center board nicely on desktop after show() request!
    QRect rctDesktop( QApplication:: desktop() -> availableGeometry() ) ;
    QRect rctFrame (topazBoard.frameGeometry()) ;
    rctFrame.moveCenter(rctDesktop.center()) ;
    topazBoard.move(rctFrame.topLeft()) ;

    // Run the application.
    return app.exec() ;
    }
