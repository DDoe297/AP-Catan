QT -= gui
QT += core
QT += network
CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Enums.cpp \
        Utility.cpp \
        board.cpp \
        city.cpp \
        game.cpp \
        harbor.cpp \
        main.cpp \
        piece.cpp \
        player.cpp \
        point.cpp \
        road.cpp \
        settlement.cpp \
        tile.cpp \
        trade.cpp \
        tradeanswer.cpp \
        server.cpp \
        worker.cpp \
        manager.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Board_FWD.hpp \
    Enums.hpp \
    Game_FWD.hpp \
    Harbor_FWD.hpp \
    Piece_FWD.hpp \
    Player_FWD.hpp \
    Point_FWD.hpp \
    Road_FWD.hpp \
    StatusCodes.hpp \
    Tile_FWD.hpp \
    TradeAnswer_FWD.hpp \
    Trade_FWD.hpp \
    Utility.hpp \
    board.hpp \
    city.hpp \
    game.hpp \
    harbor.hpp \
    piece.hpp \
    player.hpp \
    point.hpp \
    road.hpp \
    settlement.hpp \
    tile.hpp \
    trade.hpp \
    tradeanswer.hpp \
    server.hpp \
    worker.hpp \
    manager.hpp
