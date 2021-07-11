#ifndef BASE_HPP
#define BASE_HPP

#include <QObject>
#include <QVector>
#include <QMap>
#include "HexGrid.hpp"

class Tile;
class Piece{
    virtual ~Piece()=0;
};
class City: public Piece{~City();};
class Settelment: public Piece{~Settelment();};
class Road: public Piece{~Road();};
class Port;
class Base : public QObject
{
    Q_OBJECT
public:
    explicit Base(QObject *parent = nullptr);
    bool canPlacePiece(Piece *piece, int coordinate);
    void placePiece(Piece *piece, int coordinate);    //Implement converting Settelment to City
    void movePiece(Piece *piece, int originCoordinate, int destinationCoordinate);
    void removePiece(Piece *piece, int coordinate);
    QVector<Piece> getPieces(int coordiante); //Not Implemnted
    Port getPort(int tileID,QString direction); //Not Implemnted
private:
    QVector<Tile> tiles;
    QVector<Port> ports;
    QMap<HexGrid::hexgridType,QMap<int,Piece*>> pieces;
    HexGrid::hexgridType pieceTypeToHexType(Piece *piece);
};

#endif // BASE_HPP
