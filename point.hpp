#ifndef POINT_HPP
#define POINT_HPP

#include <QObject>
#include <QVector>
#include <QPair>
#include "Point_FWD.hpp"
#include "Tile_FWD.hpp"
#include "piece.hpp"

class Point : public QObject
{
    Q_OBJECT
public:
    explicit Point(QVector<Tile *> Tiles, QPair<int,int> Coordiantes, QObject *parent = nullptr);
    //To Map
    //To String
    Piece *getPiece() const;
    const QVector<Point *> &getNeighbouringPoints() const;
    const QVector<Tile *> &getTiles() const;
    void setPiece(Piece *newPiece);

private:
    QVector<Tile *> tiles;
    Piece *piece;
    QPair<int,int> coordiantes;
    QVector<Point *> neighbouringPoints;
};

#endif // POINT_HPP
