#ifndef CITY_HPP
#define CITY_HPP

#include "piece.hpp"
#include "point.hpp"

class City : public Piece {
 public:
  explicit City(Player *Owner, Point *point, QObject *parent = nullptr);
  // To Map
  // To String
  Point *getPoint() const;

 private:
  Point *point;
};
#endif  // CITY_HPP
