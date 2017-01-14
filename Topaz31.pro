QT += svg
TEMPLATE = app
TARGET = Topaz31
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += Topaz31.cpp 
SOURCES += Card.cpp CardDeck.cpp MasterDeck.cpp Players.cpp Button.cpp
SOURCES += TopazBoard.cpp TopazLogic.cpp TopazScene.cpp TopazView.cpp
HEADERS += Card.h CardDeck.h MasterDeck.h Players.h Button.h
HEADERS += TopazBoard.h TopazLogic.h TopazScene.h TopazView.h
RESOURCES += Topaz31.qrc
win32:RC_FILE = Topaz31.rc
