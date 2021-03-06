#ifndef MANAGER_H
#define MANAGER_H
#include <QJsonDocument>
#include <QObject>

#include "game.hpp"
#include "server.hpp"

class Manager : public QObject {
  Q_OBJECT
 public:
  Manager(void);
  ~Manager(void);
  void playerChoice();
  const QVector<QString> &getPlayerNames() const;
  void startServer(void);

 private:
  Game *game;
  Server *server;
  QVector<QString> playerNames;
 public slots:
  void gameTurn();
  void prepareGame();
  void addToPlayerNames(QString name);
};

#endif  // MANAGER_H
