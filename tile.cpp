#include "tile.hpp"

Tile::Tile(TileType Type, int Number, QPair<int, int> Coordiantes, QVector<Point *> Points, QObject *parent): QObject(parent)
{
    type=Type;
    number=Number;
    coordiantes=Coordiantes;
    points=Points;
}

int Tile::getNumber() const
{
    return number;
}

TileType Tile::getType() const
{
    return type;
}

void Tile::setNumber(int newNumber)
{
    number = newNumber;
}

const QVector<Point *> &Tile::getPoints() const
{
    return points;
}

void Tile::appendToPoints(Point *point)
{
    points.append(point);
}
