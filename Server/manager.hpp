#ifndef MANAGER_H
#define MANAGER_H
#include <QObject>
#include "game.hpp"
#include "server.hpp"

class Manager : public QObject
{
    Q_OBJECT
public:
    Manager(void);
    void startGame();
    void playerTurn();
    const QVector<QString> &getPlayerNames() const;
    void startServer(void);
private:
    Game *game;
    Server *server;
    QVector<QString> playerNames;
public slots:
    void prepareGame();
    void addToPlayerNames(QString name);
};

#endif // MANAGER_H
