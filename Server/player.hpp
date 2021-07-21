#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QObject>
#include <QVector>

#include "Enums.hpp"
#include "Piece_FWD.hpp"
#include "Player_FWD.hpp"
#include "StatusCodes.hpp"
#include "Utility.hpp"
#include "board.hpp"
#include "city.hpp"
#include "game.hpp"
#include "harbor.hpp"
#include "road.hpp"
#include "settlement.hpp"

class Player : public QObject {
  Q_OBJECT
 public:
  explicit Player(QString Name, Game *Game, int Num, Color Color,
                  QObject *parent = nullptr);
  const QString &getName(void) const;
  int getNum(void) const;
  Color getColor(void) const;
  StatusCode buildSettlement(Point *point, bool gameStart = false);
  StatusCode buildRoad(Point *startPoint, Point *endPoint,
                       bool gameStart = false);
  StatusCode upgradeSettlementToCity(Point *point);
  bool hasCards(QVector<ResourceCard> cardsList);
  bool hasAnyCard(void);
  int howManyOfResource(ResourceCard card);
  StatusCode removeCards(QVector<ResourceCard> cardsList);
  ResourceCard removeRandomCardAndReturnIt(void);
  StatusCode addCards(QVector<ResourceCard> cardsList);
  StatusCode buyDevelopmentCard(void);
  StatusCode removeDevelopmentCard(DevelopmentCard card);
  bool hasDevelopmentCard(DevelopmentCard card);
  StatusCode finishTurn(void);
  StatusCode checkRoadLocation(Point *startPoint, Point *endPoint);
  void checkLongestRoad(Road *newRoad);
  void checkLongestRoadUtil(Road *road, QVector<Road *> allRoads, int length);
  QVector<HarborType> getConnectedHarborTypes(void);
  QVector<Road *> getRoads(void);
  int getRoadsCount(void);
  QVector<Road *> getConnectedRoads(Point *point, QVector<Road *> roads);
  int calculateVictoryPoints(bool includeDevelopmentCards);
  const QVector<DevelopmentCard> &getDevCards() const;
  void randomlyRemoveHalfOfCards(void);
  const QVector<ResourceCard> &getCards() const;
  int getKnights() const;
  int getLongestRoadLength() const;
  QVector<Piece *> getNonRoadPieces(void);
  QVector<QPair<Point *, Point *>> getAvailableRoadCoordinates(void);
  QVector<Point *> getAvailableSettlementCoordinates(void);
  void increaseKnights(void);
  int getCities() const;
  int getSettlements() const;
  const QVector<DevelopmentCard> &getNewDevCards() const;

 private:
  QString name;
  Game *game;
  int num;
  Color color;
  int victoryPoints;
  QVector<ResourceCard> cards;
  QVector<DevelopmentCard> devCards;
  QVector<DevelopmentCard> newDevCards;
  int knights;
  int longestRoadLength;
  int roads;
  int cities;
  int settlements;
};

#endif  // PLAYER_HPP
