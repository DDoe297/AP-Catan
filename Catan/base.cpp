#include "base.hpp"

Base::Base(QObject *parent) : QObject(parent)
{

}

bool Base::canPlacePiece(Piece *piece, int coordinate)
{
    if(dynamic_cast<Road*>(piece)!=nullptr){
        return HexGrid::isLegalEdge(coordinate) && pieces[HexGrid::hexgridType::Edge].count(coordinate)==0;
    }
    else if(dynamic_cast<Settelment*>(piece)!=nullptr){
        return HexGrid::isLegalNode(coordinate) && pieces[HexGrid::hexgridType::Edge].count(coordinate)==0;
    }
    else if(dynamic_cast<City*>(piece)!=nullptr){
        return (HexGrid::isLegalNode(coordinate) && pieces[HexGrid::hexgridType::Edge].count(coordinate)==0)
                || (pieces[HexGrid::hexgridType::Edge].count(coordinate)>0 && dynamic_cast<Settelment*>(pieces[HexGrid::hexgridType::Edge][coordinate])!=nullptr);
    }
    return false;
}

void Base::placePiece(Piece *piece, int coordinate)
{
    if(canPlacePiece(piece,coordinate)){
        HexGrid::hexgridType type=pieceTypeToHexType(piece);
        pieces[type][coordinate]=piece;
    }
    //Implement converting Settelment to City
}

void Base::movePiece(Piece *piece, int originCoordinate, int destinationCoordinate)
{
    HexGrid::hexgridType type=pieceTypeToHexType(piece);
    if(pieces[type].count(originCoordinate)==0 || !canPlacePiece(piece,destinationCoordinate)){
        return;
    }
    removePiece(piece,originCoordinate);
    placePiece(piece,destinationCoordinate);
}

void Base::removePiece(Piece *piece, int coordinate)
{
    HexGrid::hexgridType type=pieceTypeToHexType(piece);
    if(pieces[type].count(coordinate)==0){
        return;
    }
    auto pos = pieces[type].find(coordinate);
    pieces[type].erase(pos);
}

HexGrid::hexgridType Base::pieceTypeToHexType(Piece *piece)
{
    if(dynamic_cast<Road*>(piece)!=nullptr){
        return HexGrid::hexgridType::Edge;
    }
    else if(dynamic_cast<Settelment*>(piece)!=nullptr || dynamic_cast<City*>(piece)!=nullptr){
        return HexGrid::hexgridType::Node;
    }
}
