#ifndef GAME_HPP
#define GAME_HPP

#include <QJsonArray>
#include <QJsonObject>
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
  explicit Game(QVector<QString> names, int numberOfPlayers = 4,
                int VictoryPointsToWin = 10, QObject *parent = nullptr);
  Board *getBoard() const;
  Player *getLargestArmyOwner() const;
  Player *getLongestRoadOwner() const;
  void checkLongestRoad(void);
  StatusCode addRoad(Player *player, Point *startPoint, Point *endPoint,
                     bool gameStart = false);
  StatusCode addSettlement(Player *player, Point *point,
                           bool gameStart = false);
  StatusCode upgradeSettlement(Player *player, Point *point);
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
  int getRoll(void);
  Player *getPlayer(int i);
  void checkForLongestRoad();
  void checkForLargestArmy();
  StatusCode playMonoploly(Player *player, ResourceCard card);
  StatusCode playeYearOfPlenty(Player *player, ResourceCard cardOne,
                               ResourceCard cardTwo);
  StatusCode playKnight(Player *player, Tile *tile, Player *victim);
  StatusCode playRoadBuilding(Player *player, QPair<Point *, Point *> firstRoad,
                              QPair<Point *, Point *> secondRoad);
  QJsonObject toJSON(void);
  bool hasCards(QVector<ResourceCard> cardsList);
  StatusCode removeCards(QVector<ResourceCard> cardsList);
  StatusCode addCards(QVector<ResourceCard> cardsList);
  const QVector<Player *> &getPlayers() const;
  void endTurn();
  int getLastRoll() const;
  int getCurrentPlayerID() const;
  int getTurnNumber() const;
  bool getStartPhase() const;
  void setStartPhase(bool newStartPhase);

private:
  Board *board;
  Player *largestArmyOwner;
  Player *longestRoadOwner;
  QVector<Player *> players;
  QVector<ResourceCard> cards;
  int victoryPointsToWin;
  QVector<DevelopmentCard> developmentCards;
  bool hasEnded;
  Player *winner;
  int lastRoll;
  int currentPlayerID;
  int turnNumber;
  bool startPhase;
};

#endif  // GAME_HPP
