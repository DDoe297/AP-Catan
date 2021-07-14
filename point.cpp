#include "point.hpp"

Point::Point(QVector<Tile *> Tiles, QPair<int, int> Coordiantes, QObject *parent): QObject(parent)
{
    tiles=Tiles;
    coordiantes=Coordiantes;
}

Piece *Point::getPiece() const
{
    return piece;
}

const QVector<Point *> &Point::getNeighbouringPoints() const
{
    return neighbouringPoints;
}

const QVector<Tile *> &Point::getTiles() const
{
    return tiles;
}

void Point::setPiece(Piece *newPiece)
{
    piece = newPiece;
}
