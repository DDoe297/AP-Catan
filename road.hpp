#ifndef ROAD_HPP
#define ROAD_HPP

#include "piece.hpp"
#include "point.hpp"

class Road : public Piece
{
public:
    explicit Road(Player *Owner,Point *StartPoint, Point *EndPoint,QObject *parent = nullptr);
    //To Map
    //To String
    Point *getStartPoint() const;
    Point *getEndPoint() const;

private:
    Point *startPoint;
    Point *endPoint;
};

#endif // ROAD_HPP
