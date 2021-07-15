#include "player.hpp"

Player::Player(QString Name, Game *Game, int Num, Color Color, QObject *parent)
    : QObject(parent) {
  name = Name;
  game = Game;
  num = Num;
  color = Color;
  victoryPoints = 0;
  knights = 0;
  longestRoadLength = 0;
  roads = 15;
  settelments = 5;
  cities = 4;
}

const QString &Player::getName() const { return name; }

int Player::getNum() const { return num; }

Color Player::getColor() const { return color; }

StatusCode Player::buildSettelment(Point *point, bool gameStart) {
  Board *gameBoard = game->getBoard();
  if (settelments == 0) {
    return StatusCode::OutOfPieces;
  }
  if (!gameStart) {
    if (!hasCards(settelmentPrice)) {
      return StatusCode::BadDeck;
    }
    if (!gameBoard->isPointConnectedToPlayerRoad(point, this)) {
      return StatusCode::IsolatedPiece;
    }
  }
  if (point->getPiece() != nullptr) {
    return StatusCode::PieceBlock;
  }
  QVector<Point *> points = point->getNeighbouringPoints(gameBoard);
  for (auto npoint : points) {
    if (npoint->getPiece() != nullptr) {
      return StatusCode::PieceBlock;
    }
  }
  if (!gameStart) {
    removeCards(settelmentPrice);
  }
  Settelment *settelment = new Settelment(this, point);
  gameBoard->addPiece(settelment, point);
  victoryPoints++;
  settelment--;
  return StatusCode::OK;
}

StatusCode Player::buildRoad(Point *startPoint, Point *endPoint,
                             bool gameStart) {
  if (roads == 0) {
    return StatusCode::OutOfPieces;
  }
  StatusCode status = checkRoadLocation(startPoint, endPoint);
  if (status != StatusCode::OK) {
    return status;
  }
  if (!gameStart) {
    if (!hasCards(roadPrice)) {
      return StatusCode::BadDeck;
    }
    removeCards(roadPrice);
  }
  Road *road = new Road(this, startPoint, endPoint);
  game->getBoard()->addRoad(road);
  checkLongestRoad(road);
  roads--;
  return StatusCode::OK;
}

StatusCode Player::upgradeSettelmentToCity(Point *point) {
  if (cities == 0) {
    return StatusCode::OutOfPieces;
  }
  Settelment *settelment = dynamic_cast<Settelment *>(point->getPiece());
  if (settelment == nullptr) {
    return StatusCode::CantUpgradeCity;
  }
  if (settelment->getOwner() != this) {
    return StatusCode::WrongOwner;
  }
  if (!hasCards(cityPrice)) {
    return StatusCode::BadDeck;
  }
  removeCards(cityPrice);
  delete settelment;
  City *newCity = new City(this, point);
  point->setPiece(newCity);
  victoryPoints++;
  cities--;
  settelments++;
  return StatusCode::OK;
}

bool Player::hasCards(QVector<ResourceCard> cardsList) {
  QVector<ResourceCard> cardsCopy(cards);
  for (auto card : cardsList) {
    if (!cardsCopy.contains(card)) {
      return false;
    }
    cardsCopy.removeOne(card);
  }
  return true;
}

bool Player::hasAnyCard() { return !cards.isEmpty(); }

StatusCode Player::removeCards(QVector<ResourceCard> cardsList) {
  if (!hasCards(cardsList)) {
    return StatusCode::BadDeck;
  }
  for (auto card : cardsList) {
    cards.removeOne(card);
  }
  return StatusCode::OK;
}

ResourceCard Player::removeRandomCardAndReturnIt() {
  int index = randomNumber(0, cards.length() - 1);
  ResourceCard card = cards[index];
  cards.removeAt(index);
  return card;
}

StatusCode Player::addCards(QVector<ResourceCard> cardsList) {
  for (auto card : cardsList) {
    cards.append(card);
  }
  return StatusCode::OK;
}

StatusCode Player::buyDevelopmentCard(void) {
  QVector<DevelopmentCard> gameDevCards = game->getDevelopmentCards();
  if (gameDevCards.length() == 0) {
    return StatusCode::BadDeck;
  }
  if (!hasCards(devCardPrice)) {
    return StatusCode::BadDeck;
  }
  removeCards(devCardPrice);
  newDevCards.append(gameDevCards.last());
  gameDevCards.removeLast();
  return StatusCode::OK;
}

StatusCode Player::removeDevelopmentCard(DevelopmentCard card) {
  if (hasDevelopmentCard(card)) {
    devCards.removeOne(card);
  }
  return StatusCode::BadCards;
}

bool Player::hasDevelopmentCard(DevelopmentCard card) {
  return devCards.contains(card);
}

StatusCode Player::finishTurn() {
  for (auto card : newDevCards) {
    devCards.append(card);
  }
  newDevCards.clear();
  return StatusCode::OK;
}

StatusCode Player::checkRoadLocation(Point *startPoint, Point *endPoint) {
  Board *gameBoard = game->getBoard();
  QVector<Road *> roads = gameBoard->getRoads();
  if (!gameBoard->arePointsConnected(startPoint, endPoint)) {
    return StatusCode::NotConnected;
  }
  if (gameBoard->isThereARoadBetweenTwoPoints(startPoint, endPoint)) {
    return StatusCode::PieceBlock;
  }
  if (!gameBoard->isRoadConnectedToAPiece(startPoint, endPoint, this)) {
    return StatusCode::IsolatedPiece;
  }
  return StatusCode::OK;
}

void Player::checkLongestRoad(Road *newRoad) {
  QVector<Road *> roads = getRoads();
  roads.removeAll(newRoad);
  checkLongestRoadUtil(newRoad, roads, 1);
}

void Player::checkLongestRoadUtil(Road *road, QVector<Road *> allRoads,
                                  int length) {
  QVector<Point *> points = {road->getStartPoint(), road->getEndPoint()};
  for (auto point : points) {
    QVector<Road *> connectedRoads = getConnectedRoads(point, allRoads);
    if (connectedRoads.length() == 0) {
      if (length > longestRoadLength) {
        longestRoadLength = length;
      } else {
        for (auto road : connectedRoads) {
          if (allRoads.contains(road)) {
            QVector<Road *> roadsCopy(allRoads);
            roadsCopy.removeAll(road);
            checkLongestRoadUtil(road, roadsCopy, length + 1);
          }
        }
      }
    }
  }
}

QVector<HarborType> Player::getConnectedHarborTypes() {
  QVector<HarborType> connectedHarbors;
  QVector<Harbor *> allHarbors = game->getBoard()->getHarbors();
  QVector<Piece *> pieces = game->getBoard()->getPieces();
  for (auto piece : pieces) {
    if (piece->getOwner() == this) {
      Settelment *settelment = dynamic_cast<Settelment *>(piece);
      if (settelment != nullptr) {
        for (auto harbor : allHarbors) {
          if (settelment->getPoint() == harbor->getStartPoint() ||
              settelment->getPoint() == harbor->getEndPoint()) {
            if (!connectedHarbors.contains(harbor->getType())) {
              connectedHarbors.append(harbor->getType());
            }
          }
        }
      } else {
        City *city = dynamic_cast<City *>(piece);
        for (auto harbor : allHarbors) {
          if (city->getPoint() == harbor->getStartPoint() ||
              city->getPoint() == harbor->getEndPoint()) {
            if (!connectedHarbors.contains(harbor->getType())) {
              connectedHarbors.append(harbor->getType());
            }
          }
        }
      }
    }
  }
  return connectedHarbors;
}

QVector<Road *> Player::getRoads() {
  QVector<Road *> allRoads = game->getBoard()->getRoads();
  QVector<Road *> roads;
  for (auto road : allRoads) {
    if (road->getOwner() == this) {
      roads.append(road);
    }
  }
  return roads;
}

QVector<Road *> Player::getConnectedRoads(Point *point, QVector<Road *> roads) {
  QVector<Road *> connectedRoads;
  for (auto road : roads) {
    if (road->getStartPoint() == point || road->getEndPoint() == point) {
      connectedRoads.append(road);
    }
  }
  return connectedRoads;
}

int Player::calculateVictoryPoints(bool includeDevelopmentCards) {
  int points = victoryPoints;
  if (game->getLongestRoadOwner() == this) {
    points += 2;
  }
  if (game->getLargestArmyOwner() == this) {
    points += 2;
  }
  if (includeDevelopmentCards) {
    for (auto card : devCards) {
      if (card == DevelopmentCard::VictoryPoint) {
        points += 1;
      }
    }
  }
  return points;
}

const QVector<DevelopmentCard> &Player::getDevCards() const { return devCards; }

void Player::randomlyRemoveHalfOfCards() {
  if (cards.length() <= 7) {
    return;
  }
  int cardsLength = cards.length();
  for (int i = 0; i < cardsLength / 2; i++) {
    cards.removeAt(randomNumber(0, cards.length() - 1));
  }
}

const QVector<ResourceCard> &Player::getCards() const { return cards; }

int Player::getKnights() const { return knights; }

int Player::getLongestRoadLength() const { return longestRoadLength; }
