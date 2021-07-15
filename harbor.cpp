#include "harbor.hpp"

ResourceCard Harbor::harborToResourceCard(HarborType type) {
  switch (type) {
    case HarborType::Lumber:
      return ResourceCard::Lumber;
    case HarborType::Brick:
      return ResourceCard::Brick;
    case HarborType::Grain:
      return ResourceCard::Grain;
    case HarborType::Ore:
      return ResourceCard::Ore;
    case HarborType::Wool:
      return ResourceCard::Wool;
    case HarborType::Any:
      return ResourceCard::None;
  }
  return ResourceCard::None;
}

Harbor::Harbor(Point *StartPoint, Point *EndPoint, HarborType Type,
               QObject *parent)
    : QObject(parent) {
  startPoint = StartPoint;
  endPoint = EndPoint;
  type = Type;
}

QVector<Point *> Harbor::getPoints() {
  QVector<Point *> points = {startPoint, endPoint};
  return points;
}

Point *Harbor::getStartPoint() const { return startPoint; }

Point *Harbor::getEndPoint() const { return endPoint; }

HarborType Harbor::getType() const { return type; }
