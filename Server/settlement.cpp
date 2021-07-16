#include "settlement.hpp"

Settlement::Settlement(Player *Owner, Point *Point, QObject *parent)
    : Piece(Owner, PieceType::Settlement, parent) {
  point = Point;
}

Point *Settlement::getPoint() const { return point; }
