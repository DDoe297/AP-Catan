#ifndef HARBOR_HPP
#define HARBOR_HPP

#include <QObject>
#include <QVector>

#include "Enums.hpp"
#include "Harbor_FWD.hpp"
#include "Point_FWD.hpp"
#include "point.hpp"

class Harbor : public QObject {
  Q_OBJECT
 public:
  static ResourceCard harborToResourceCard(HarborType type);
  explicit Harbor(Point *StartPoint, Point *EndPoint, HarborType Type,
                  QObject *parent = nullptr);
  // To Map
  // To String
  QVector<Point *> getPoints(void);
  Point *getStartPoint() const;
  Point *getEndPoint() const;
  HarborType getType() const;

 private:
  Point *startPoint;
  Point *endPoint;
  HarborType type;
};

#endif  // HARBOR_HPP
