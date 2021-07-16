#ifndef BOARD_HPP
#define BOARD_HPP

#include <QObject>
#include <QPair>
#include <QSet>
#include <QVector>

#include "Board_FWD.hpp"
#include "Game_FWD.hpp"
#include "Harbor_FWD.hpp"
#include "Piece_FWD.hpp"
#include "Player_FWD.hpp"
#include "Point_FWD.hpp"
#include "StatusCodes.hpp"
#include "Utility.hpp"
#include "city.hpp"
#include "game.hpp"
#include "harbor.hpp"
#include "player.hpp"
#include "road.hpp"
#include "settlement.hpp"
#include "tile.hpp"

class Board : public QObject {
  Q_OBJECT
 public:
  explicit Board(Game *Game, QObject *parent = nullptr);
  QVector<Piece *> getPieces(void);
  StatusCode addPiece(Piece *piece, Point *Point);
  StatusCode addRoad(Piece *piece);
  bool arePointsConnected(Point *startPoint, Point *endPoint);
  bool isPointConnectedToPlayerRoad(Point *point, Player *owner);
  bool isThereARoadBetweenTwoPoints(Point *pointOne, Point *pointTwo);
  bool isRoadConnectedToAPiece(Point *startPoint, Point *endPoint,
                               Player *owner);
  StatusCode addResource(int roll);
  const QVector<Road *> &getRoads() const;
  const QVector<Harbor *> &getHarbors() const;
  StatusCode addInitialResources(Piece *piece);
  Road *getRoad(Point *startPoint, Point *endPoint);
  void moveRobber(Tile *tile);
  static QVector<TileType> randomTileList(void);
  static QVector<int> randomNumList(void);
  static QVector<HarborType> randomHarborList();
  static bool checkSixAndEightAdjacency(QVector<int> nums);
  const QVector<QVector<Point *>> &getPoints() const;
  const QVector<QVector<Tile *>> &getTiles() const;
  QVector<Point *> getAllPoints(void);
  QVector<Tile *> getAllTiles(void);
  QVector<Point *> borderPoints(void);
  QVector<Player *> playersWhoOwnSettelmentAroundTile(Tile *tile);
  bool doesPlayerOwnSettelmentAroundTile(Tile *tile, Player *player);
  Tile *getTile(int i, int j);
  Point *getPoint(int i, int j);
  Tile *getRobber() const;

private:
  Game *game;
  QVector<QVector<Point *>> points;
  QVector<QVector<Tile *>> tiles;
  QVector<Road *> roads;
  QVector<Harbor *> harbors;
  Tile *robber;
};

#endif  // BOARD_HPP
