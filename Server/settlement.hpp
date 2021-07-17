#ifndef SETTLEMENT_HPP
#define SETTLEMENT_HPP

#include "piece.hpp"
#include "point.hpp"

class Settlement : public Piece {
 public:
  explicit Settlement(Player *Owner, Point *point, QObject *parent = nullptr);
  Point *getPoint() const;

 private:
  Point *point;
};

#endif  // SETTLEMENT_HPP
