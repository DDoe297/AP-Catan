#ifndef POINT_HPP
#define POINT_HPP

#include <QObject>
#include <QPair>
#include <QVector>

#include "Board_FWD.hpp"
#include "Point_FWD.hpp"
#include "Tile_FWD.hpp"
#include "board.hpp"
#include "piece.hpp"

class Point : public QObject {
  Q_OBJECT
 public:
  explicit Point(QVector<Tile *> Tiles, QPair<int, int> Coordiantes,
                 QObject *parent = nullptr);
  // To Map
  // To String
  Piece *getPiece() const;
  QVector<Point *> getNeighbouringPoints(Board *board);
  QVector<Tile *> getTiles(Board *board);
  void setPiece(Piece *newPiece);

 private:
  QVector<Tile *> tiles;
  Piece *piece;
  QPair<int, int> coordiantes;
};

#endif  // POINT_HPP
