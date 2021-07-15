#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>

#include "Board_FWD.hpp"
#include "Enums.hpp"
#include "Game_FWD.hpp"
#include "Player_FWD.hpp"
#include "board.hpp"
#include "tile.hpp"

class Game : public QObject {
  Q_OBJECT
 public:
  explicit Game(int numberOfPlayers = 4, int VictoryPointsToWin = 10,
                QObject *parent = nullptr);
  Board *getBoard() const;
  Player *getLargestArmyOwner() const;
  Player *getLongestRoadOwner() const;
  void checkLongestRoad(void);
  StatusCode addRoad(Player *player, Point *startPoint, Point *endPoint,
                     bool gameStart = false);
  StatusCode addSettelment(Player *player, Point *point,
                           bool gameStart = false);
  StatusCode upgradeSettelment(Player *player, Point *point);
  bool checkForWinner(void);
  bool getHasEnded() const;
  Player *getWinner() const;
  QVector<DevelopmentCard> &getDevelopmentCards();
  StatusCode trade(Player *getter, Player *giver,
                   QVector<ResourceCard> getCards,
                   QVector<ResourceCard> giveCards);
  StatusCode tradeWithBank(Player *getter, ResourceCard getCard,
                           QVector<ResourceCard> giveCards);
  StatusCode activateRobber(Tile *tile, Player *player, Player *victim);
  static int getRoll(void);
  Player *getPlayer(int i);
  void checkForLongestRoad();
  void checkForLargestArmy();

 private:
  Board *board;
  Player *largestArmyOwner;
  Player *longestRoadOwner;
  QVector<Player *> players;
  int victoryPointsToWin;
  QVector<DevelopmentCard> developmentCards;
  bool hasEnded;
  Player *winner;
};

#endif  // GAME_HPP
