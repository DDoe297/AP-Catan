#include "settelment.hpp"

Settelment::Settelment(Player *Owner, Point *Point, QObject *parent) : Piece(Owner,PieceType::Settelment, parent)
{
    point=Point;
}

Point *Settelment::getPoint() const
{
    return point;
}
