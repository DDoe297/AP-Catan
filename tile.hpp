#ifndef TILE_HPP
#define TILE_HPP

#include <QObject>
#include <QPair>
#include <QVector>

#include "Enums.hpp"
#include "Point_FWD.hpp"
#include "Tile_FWD.hpp"

class Tile : public QObject {
  Q_OBJECT
 public:
  explicit Tile(TileType type, int number, QPair<int, int> coordiantes,
                QVector<Point *> points, QObject *parent = nullptr);
  // To Map
  // To String
  int getNumber() const;
  TileType getType() const;
  void setNumber(int newNumber);
  const QVector<Point *> &getPoints() const;
  void appendToPoints(Point *point);
  const QPair<int, int> &getCoordiantes() const;

private:
  TileType type;
  int number;
  QPair<int, int> coordiantes;
  QVector<Point *> points;
};

#endif  // TILE_HPP
