#include "board.hpp"

Board::Board(Game *Game, QObject *parent) : QObject(parent) {
  game = Game;
  QVector<TileType> tileTypes = randomTileList();
  QVector<int> nums = randomNumList();
  QVector<int> tileSizeRefrence = {3, 4, 5, 4, 3};
  for (int i = 0; i < 5; i++) {
    QVector<Tile *> tempTiles;
    for (int j = 0; j < tileSizeRefrence[i]; j++) {
      Tile *newTile = new Tile(tileTypes.last(), 0, {i, j}, {});
      tileTypes.pop_back();
      if (newTile->getType() == TileType::Desert) {
        robber = newTile;
      } else {
        newTile->setNumber(nums.last());
        nums.pop_back();
      }
      tempTiles.append(newTile);
    }
    tiles.append(tempTiles);
  }
  QVector<int> pointSizeRefrence = {7, 9, 11, 11, 9, 7};
  for (int i = 0; i < 6; i++) {
    QVector<Point *> tempPoints;
    for (int j = 0; j < pointSizeRefrence[i]; j++) {
      Point *newPoint = new Point({i, j});
      for (auto tile : newPoint->getTiles(this)) {
        tile->appendToPoints(newPoint);
      }
      tempPoints.append(newPoint);
    }
    points.append(tempPoints);
  }
  QVector<Point *> border = borderPoints();
  QVector<int> pattern = {1, 2, 1};
  int index = 0;
  QVector<HarborType> boardHarbors(harborList);
  shuffle(boardHarbors);
  while (!boardHarbors.isEmpty()) {
    Point *one = border.last();
    border.removeLast();
    Point *two = border.last();
    border.removeLast();
    HarborType type = boardHarbors.last();
    boardHarbors.removeLast();
    Harbor *harbor = new Harbor(one, two, type);
    harbors.append(harbor);
    for (int i = 0; i < pattern[index % pattern.length()]; i++) {
      border.removeLast();
    }
    index++;
  }
}

Board::~Board() {
  for (const auto &row : tiles) {
    for (auto &tile : row) {
      delete tile;
    }
  }
  for (auto harbor : harbors) {
    delete harbor;
  }
  for (const auto &row : points) {
    for (auto point : row) {
      if (point->getPiece() != nullptr) {
        delete point->getPiece();
      }
      delete point;
    }
  }
  for (auto road : roads) {
    delete road;
  }
}

QVector<Piece *> Board::getPieces() {
  QVector<Piece *> pieces;
  for (const auto &row : points) {
    for (auto point : row) {
      if (point->getPiece() != nullptr) {
        pieces.append(point->getPiece());
      }
    }
  }
  return pieces;
}

StatusCode Board::addPiece(Piece *piece, Point *Point) {
  Point->setPiece(piece);
  return StatusCode::OK;
}

StatusCode Board::addRoad(Piece *piece) {
  roads.append(dynamic_cast<Road *>(piece));
  return StatusCode::OK;
}

bool Board::arePointsConnected(Point *startPoint, Point *endPoint) {
  for (auto point : startPoint->getNeighbouringPoints(game->getBoard())) {
    if (endPoint == point) {
      return true;
    }
  }
  return false;
}

bool Board::isPointConnectedToPlayerRoad(Point *point, Player *owner) {
  QVector<Road *> roads = getRoads();
  for (auto road : roads) {
    if ((road->getStartPoint() == point || road->getEndPoint() == point) &&
        road->getOwner() == owner) {
      return true;
    }
  }
  return false;
}

bool Board::isThereARoadBetweenTwoPoints(Point *pointOne, Point *pointTwo) {
  for (auto road : roads) {
    if ((road->getStartPoint() == pointOne &&
         road->getEndPoint() == pointTwo) ||
        (road->getEndPoint() == pointOne &&
         road->getStartPoint() == pointTwo)) {
      return true;
    }
  }
  return false;
}

bool Board::isRoadConnectedToAPiece(Point *startPoint, Point *endPoint,
                                    Player *owner) {
  if ((startPoint->getPiece() != nullptr &&
       startPoint->getPiece()->getOwner() == owner) ||
      (endPoint->getPiece() != nullptr &&
       endPoint->getPiece()->getOwner() == owner)) {
    return true;
  }
  for (auto road : roads) {
    if (road->getStartPoint() == startPoint ||
        road->getEndPoint() == startPoint) {
      if (startPoint->getPiece() == nullptr ||
          (startPoint->getPiece()->getOwner() == owner)) {
        return true;
      }
    } else if (road->getStartPoint() == endPoint ||
               road->getEndPoint() == endPoint) {
      if (endPoint->getPiece() == nullptr ||
          (endPoint->getPiece()->getOwner() == owner)) {
        return true;
      }
    }
  }
  return false;
}

StatusCode Board::addResource(int roll) {
  QMap<ResourceCard, QVector<ResourceCard>> allCardsNeeded;
  QHash<Player *, QMap<ResourceCard, QVector<ResourceCard>>>
      cardsNeededForEachPlayer;
  for (const auto &point : getAllPoints()) {
    if (point->getPiece() != nullptr) {
      QVector<Tile *> tiles = point->getTiles(this);
      Settlement *settlement = dynamic_cast<Settlement *>(point->getPiece());
      City *city = dynamic_cast<City *>(point->getPiece());
      if (settlement != nullptr) {
        for (auto tile : tiles) {
          if (robber == tile) {
            continue;
          }
          if (tile->getNumber() == roll) {
            ResourceCard type = TileTypeToResource[tile->getType()];
            allCardsNeeded[type].append(type);
            cardsNeededForEachPlayer[settlement->getOwner()][type].append(type);
          }
        }
      } else if (city != nullptr) {
        for (auto tile : tiles) {
          if (robber == tile) {
            continue;
          }
          if (tile->getNumber() == roll) {
            ResourceCard type = TileTypeToResource[tile->getType()];
            allCardsNeeded[type].append({type, type});
            cardsNeededForEachPlayer[city->getOwner()][type].append(
                {type, type});
          }
        }
      }
    }
  }
  for (int i = 0; i < 5; i++) {
    if (game->hasCards(allCardsNeeded[(ResourceCard)i])) {
      for (auto player : game->getPlayers()) {
        player->addCards(cardsNeededForEachPlayer[player][(ResourceCard)i]);
        game->removeCards(cardsNeededForEachPlayer[player][(ResourceCard)i]);
      }
    }
  }
  return StatusCode::OK;
}

const QVector<Road *> &Board::getRoads() const { return roads; }

const QVector<Harbor *> &Board::getHarbors() const { return harbors; }

StatusCode Board::addInitialResources(Piece *piece) {
  Settlement *settlement = dynamic_cast<Settlement *>(piece);
  if (settlement != nullptr) {
    for (auto tile : settlement->getPoint()->getTiles(this)) {
      piece->getOwner()->addCards({TileTypeToResource[tile->getType()]});
      game->removeCards({TileTypeToResource[tile->getType()]});
    }
    return StatusCode::OK;
  } else {
    return StatusCode::PieceDosentExist;
  }
  return StatusCode::OK;
}

Road *Board::getRoad(Point *startPoint, Point *endPoint) {
  for (auto road : roads) {
    if ((road->getStartPoint() == startPoint &&
         road->getEndPoint() == endPoint) ||
        (road->getStartPoint() == endPoint &&
         road->getEndPoint() == startPoint)) {
      return road;
    }
  }
  return nullptr;
}

void Board::moveRobber(Tile *tile) { robber = tile; }

QVector<TileType> Board::randomTileList() {
  QVector<TileType> tiles(TileList);
  shuffle(tiles);
  return tiles;
}

QVector<int> Board::randomNumList() {
  QVector<int> nums(NumList);
  shuffle(nums);
  while (checkSixAndEightAdjacency(nums)) {
    shuffle(nums);
  }
  return nums;
}

bool Board::checkSixAndEightAdjacency(QVector<int> nums) {
  for (int i = 0; i < nums.length() - 1; i++) {
    if ((nums[i] == 6 && nums[i + 1] == 8) ||
        (nums[i] == 8 && nums[i + 1] == 6)) {
      return true;
    }
  }
  return false;
}

const QVector<QVector<Point *>> &Board::getPoints() const { return points; }

const QVector<QVector<Tile *>> &Board::getTiles() const { return tiles; }

QVector<Point *> Board::getAllPoints() {
  QVector<Point *> allPoints;
  for (const auto &row : points) {
    for (auto point : row) {
      allPoints.append(point);
    }
  }
  return allPoints;
}

QVector<Tile *> Board::getAllTiles() {
  QVector<Tile *> allTiles;
  for (const auto &row : tiles) {
    for (auto tile : row) {
      allTiles.append(tile);
    }
  }
  return allTiles;
}

QVector<Point *> Board::borderPoints() {
  QVector<int> pointSizeRefrence = {7, 9, 11, 11, 9, 7};
  QVector<Point *> bottom;
  for (int i = 0; i < pointSizeRefrence.last(); i++) {
    bottom.append(points[pointSizeRefrence.length() - 1][i]);
  }
  QVector<Point *> top;
  for (int i = 0; i < pointSizeRefrence.first(); i++) {
    top.append(points[0][i]);
  }
  QVector<Point *> right;
  QVector<Point *> left;
  for (int i = 1; i < pointSizeRefrence.length() - 1; i++) {
    QVector<Point *> firstTwo;
    for (int j = 1; j >= 0; j--) {
      firstTwo.append(points[i][j]);
    }
    QVector<Point *> lastTwo;
    for (int j = pointSizeRefrence[i] - 2; j < pointSizeRefrence[i]; j++) {
      lastTwo.append(points[i][j]);
    }
    if (i > (pointSizeRefrence.length() - 1) / 2) {
      std::reverse(firstTwo.begin(), firstTwo.end());
      std::reverse(lastTwo.begin(), lastTwo.end());
    }
    append(left, firstTwo);
    append(right, lastTwo);
  }
  std::reverse(bottom.begin(), bottom.end());
  std::reverse(left.begin(), left.end());
  QVector<Point *> borderPoints;
  append(borderPoints, top);
  append(borderPoints, right);
  append(borderPoints, bottom);
  append(borderPoints, left);
  return borderPoints;
}

QVector<Player *> Board::playersWhoOwnSettlementAroundTile(Tile *tile) {
  QVector<Player *> players;
  QVector<Point *> points = tile->getPoints();
  for (auto point : points) {
    if (point->getPiece() != nullptr &&
        !players.contains(point->getPiece()->getOwner())) {
      players.append(point->getPiece()->getOwner());
    }
  }
  return players;
}

bool Board::doesPlayerOwnSettlementAroundTile(Tile *tile, Player *player) {
  for (auto point : tile->getPoints()) {
    if (point->getPiece() != nullptr) {
      if (player == point->getPiece()->getOwner()) {
        return true;
      }
    }
  }
  return false;
}

Tile *Board::getTile(int i, int j) {
  if (i < tiles.length() && j < tiles[i].length()) {
    return tiles[i][j];
  }
  return nullptr;
}

Point *Board::getPoint(int i, int j) {
  if (i < points.length() && j < points[i].length()) {
    return points[i][j];
  }
  return nullptr;
}

Tile *Board::getRobber() const { return robber; }
