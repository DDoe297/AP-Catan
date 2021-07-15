#include "point.hpp"

Point::Point(QVector<Tile *> Tiles, QPair<int, int> Coordiantes,
             QObject *parent)
    : QObject(parent) {
  tiles = Tiles;
  coordiantes = Coordiantes;
}

Piece *Point::getPiece() const { return piece; }

QVector<Point *> Point::getNeighbouringPoints(Board *board) {
  QVector<QVector<Point *>> boardPoints = board->getPoints();
  QVector<Point *> neighbouringPoints;
  if (coordiantes.second > 0) {
    neighbouringPoints.append(
        boardPoints[coordiantes.first][coordiantes.second - 1]);
  }
  if (coordiantes.second < boardPoints[coordiantes.first].length() - 1) {
    neighbouringPoints.append(
        boardPoints[coordiantes.first][coordiantes.second + 1]);
  }
  if (coordiantes.first == 2 && coordiantes.second % 2 == 0) {
    neighbouringPoints.append(
        boardPoints[coordiantes.first + 1][coordiantes.second]);
  } else if (coordiantes.first == 3 && coordiantes.second % 2 == 0) {
    neighbouringPoints.append(
        boardPoints[coordiantes.first - 1][coordiantes.second]);
  } else if (coordiantes.first <= boardPoints.length() / 2) {
    if (coordiantes.second % 2 == 0) {
      neighbouringPoints.append(
          boardPoints[coordiantes.first + 1][coordiantes.second + 1]);
    } else if (coordiantes.first > 0 && coordiantes.second > 0) {
      neighbouringPoints.append(
          boardPoints[coordiantes.first - 1][coordiantes.second - 1]);
    }
  } else {
    if (coordiantes.second % 2 == 0) {
      neighbouringPoints.append(
          boardPoints[coordiantes.first - 1][coordiantes.second + 1]);
    } else if (coordiantes.first < boardPoints.length() - 1 &&
               coordiantes.second > 0) {
      neighbouringPoints.append(
          boardPoints[coordiantes.first + 1][coordiantes.second - 1]);
    }
  }
  return neighbouringPoints;
}

QVector<Tile *> Point::getTiles(Board *board) {
  QVector<Tile *> tiles;
  QVector<QVector<Tile *>> boardTiles = board->getTiles();
  QVector<int> pointSizeRefrence = {7, 9, 11, 11, 9, 7};
  if (coordiantes.first < pointSizeRefrence.length() / 2) {
    if (coordiantes.second < pointSizeRefrence[coordiantes.first] - 1) {
      tiles.append(boardTiles[coordiantes.first][coordiantes.second / 2]);
    }
    if (coordiantes.second > 0 && coordiantes.second % 2 == 0) {
      tiles.append(boardTiles[coordiantes.first][coordiantes.second / 2 - 1]);
    }
    if (coordiantes.first > 0) {
      if (coordiantes.second > 0 &&
          coordiantes.second < pointSizeRefrence[coordiantes.first] - 2) {
        tiles.append(
            boardTiles[coordiantes.first - 1][(coordiantes.second - 1) / 2]);
      }
      if (coordiantes.second > 1 &&
          coordiantes.second < pointSizeRefrence[coordiantes.first] - 1 &&
          coordiantes.second % 2) {
        tiles.append(boardTiles[coordiantes.first - 1]
                               [(coordiantes.second - 1) / 2 - 1]);
      }
    }
  } else {
    if (coordiantes.first < pointSizeRefrence.length() - 1) {
      if (coordiantes.second > 0 &&
          coordiantes.second < pointSizeRefrence[coordiantes.first] - 2) {
        tiles.append(
            boardTiles[coordiantes.first][(coordiantes.second - 1) / 2]);
      }
      if (coordiantes.second > 1 &&
          coordiantes.second < pointSizeRefrence[coordiantes.first] &&
          coordiantes.second % 2) {
        tiles.append(
            boardTiles[coordiantes.first][(coordiantes.second - 1) / 2 - 1]);
      }
    }
    if (coordiantes.second < pointSizeRefrence[coordiantes.first] - 1) {
      tiles.append(boardTiles[coordiantes.first - 1][coordiantes.second / 2]);
    }
    if (coordiantes.second > 1 && coordiantes.second % 2 == 0) {
      tiles.append(
          boardTiles[coordiantes.first - 1][(coordiantes.second - 1) / 2]);
    }
  }
  return tiles;
}

void Point::setPiece(Piece *newPiece) { piece = newPiece; }
