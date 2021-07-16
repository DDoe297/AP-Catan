#ifndef ROAD_HPP
#define ROAD_HPP

#include "Piece_FWD.hpp"
#include "Point_FWD.hpp"
#include "Road_FWD.hpp"
#include "piece.hpp"
#include "point.hpp"

class Road : public Piece {
 public:
  explicit Road(Player *Owner, Point *StartPoint, Point *EndPoint,
                QObject *parent = nullptr);
  Point *getStartPoint() const;
  Point *getEndPoint() const;

 private:
  Point *startPoint;
  Point *endPoint;
};

#endif  // ROAD_HPP
