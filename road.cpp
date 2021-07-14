#include "road.hpp"

Road::Road(Player *Owner, Point *StartPoint, Point *EndPoint, QObject *parent) : Piece(Owner,PieceType::Road, parent)
{
    startPoint=StartPoint;
    endPoint=EndPoint;
}

Point *Road::getStartPoint() const
{
    return startPoint;
}

Point *Road::getEndPoint() const
{
    return endPoint;
}
