#ifndef MANAGER_H
#define MANAGER_H
#include "game.hpp"
#include <QObject>

class Manager
{
public:
    QVector<initialPlayerData> d;
    Game _game=Game(d);
    Manager();
    void preparingGame();
    void startGame();
    void playerTurn();
};

#endif // MANAGER_H
