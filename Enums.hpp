#ifndef ENUMS_HPP
#define ENUMS_HPP
#include <QMap>
#include <QVector>

enum class PieceType { Settelment = 0, Road = 1, City = 2 };

enum class TileType {
  Forest = 0,
  Hill = 1,
  Mountain = 2,
  Pasture = 3,
  Field = 4,
  Desert = 5
};

enum class ResourceCard {
  None = -1,
  Lumber = 0,
  Brick = 1,
  Ore = 2,
  Wool = 3,
  Grain = 4
};

enum class HarborType {
  Lumber = 0,
  Brick = 1,
  Ore = 2,
  Wool = 3,
  Grain = 4,
  Any = 5
};

enum class DevelopmentCard {
  RoadBuilding = 0,
  VictoryPoint = 1,
  Knight = 2,
  Monopoly = 3,
  YearOfPlenty = 4
};

enum class Color { Red = 0, Blue = 1, White = 2, Orange = 3 };

const QVector<ResourceCard> settelmentPrice = {
    ResourceCard::Wool, ResourceCard::Lumber, ResourceCard::Brick,
    ResourceCard::Grain};

const QVector<ResourceCard> roadPrice = {
    ResourceCard::Lumber,
    ResourceCard::Brick,
};

const QVector<ResourceCard> cityPrice = {ResourceCard::Grain,
                                         ResourceCard::Grain, ResourceCard::Ore,
                                         ResourceCard::Ore, ResourceCard::Ore};

const QVector<ResourceCard> devCardPrice = {
    ResourceCard::Grain, ResourceCard::Ore, ResourceCard::Wool};

const QMap<TileType, ResourceCard> TileTypeToResource = {
    {TileType::Forest, ResourceCard::Lumber},
    {TileType::Hill, ResourceCard::Brick},
    {TileType::Mountain, ResourceCard::Ore},
    {TileType::Pasture, ResourceCard::Wool},
    {TileType::Field, ResourceCard::Grain},
};

const QVector<TileType> TileList = {
    TileType::Field,    TileType::Field,    TileType::Field,
    TileType::Field,    TileType::Forest,   TileType::Forest,
    TileType::Forest,   TileType::Forest,   TileType::Pasture,
    TileType::Pasture,  TileType::Pasture,  TileType::Pasture,
    TileType::Hill,     TileType::Hill,     TileType::Hill,
    TileType::Mountain, TileType::Mountain, TileType::Mountain,
    TileType::Desert};

const QVector<int> NumList = {2,  3, 4, 5, 6, 8, 9, 10, 11,
                              12, 3, 4, 5, 6, 8, 9, 10, 11};

const QVector<HarborType> harborList = {
    HarborType::Lumber, HarborType::Brick, HarborType::Ore,
    HarborType::Grain,  HarborType::Wool,  HarborType::Any,
    HarborType::Any,    HarborType::Any,   HarborType::Any};

const QVector<DevelopmentCard> devCards = {
    DevelopmentCard::RoadBuilding, DevelopmentCard::Monopoly,
    DevelopmentCard::YearOfPlenty, DevelopmentCard::VictoryPoint,
    DevelopmentCard::Knight,       DevelopmentCard::RoadBuilding,
    DevelopmentCard::Monopoly,     DevelopmentCard::YearOfPlenty,
    DevelopmentCard::VictoryPoint, DevelopmentCard::Knight,
    DevelopmentCard::VictoryPoint, DevelopmentCard::Knight,
    DevelopmentCard::VictoryPoint, DevelopmentCard::Knight,
    DevelopmentCard::VictoryPoint, DevelopmentCard::Knight,
    DevelopmentCard::Knight,       DevelopmentCard::Knight,
    DevelopmentCard::Knight,       DevelopmentCard::Knight,
    DevelopmentCard::Knight,       DevelopmentCard::Knight,
    DevelopmentCard::Knight,       DevelopmentCard::Knight,
    DevelopmentCard::Knight,
};
#endif  // ENUMS_HPP
