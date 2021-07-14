#include "city.hpp"

City::City(Player *Owner, Point *Point, QObject *parent) : Piece(Owner,PieceType::City, parent)
{
    point=Point;
}

Point *City::getPoint() const
{
    return point;
}
