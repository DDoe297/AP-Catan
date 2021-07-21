#include "game.hpp"

Game::Game(QVector<QString> names, int numberOfPlayers, int VictoryPointsToWin,
           QObject *parent)
    : QObject(parent) {
  board = new Board(this);
  for (int i = 0; i < numberOfPlayers; i++) {
    Player *newPlayer = new Player(names[i], this, i, (Color)i);
    players.append(newPlayer);
  }
  victoryPointsToWin = VictoryPointsToWin;
  developmentCards = QVector<DevelopmentCard>(devCards);
  shuffle(developmentCards);
  cards = gameDeck();
  hasEnded = false;
  longestRoadOwner = nullptr;
  largestArmyOwner = nullptr;
  winner = nullptr;
  currentPlayerID = 0;
  lastRoll = {0, 0};
  turnNumber = 0;
  startPhase = true;
  tradeHolder = nullptr;
  hasRolled = false;
}

Game::~Game() {
  delete board;
  for (auto player : players) {
    delete player;
  }
}

Board *Game::getBoard() const { return board; }

Player *Game::getLargestArmyOwner() const { return largestArmyOwner; }

Player *Game::getLongestRoadOwner() const { return longestRoadOwner; }

StatusCode Game::addRoad(Player *player, Point *startPoint, Point *endPoint,
                         bool gameStart) {
  StatusCode code = player->buildRoad(startPoint, endPoint, gameStart);
  if (code == StatusCode::OK) {
    checkForLongestRoad();
  }
  return code;
}

StatusCode Game::addSettlement(Player *player, Point *point, bool gameStart) {
  StatusCode code = player->buildSettlement(point, gameStart);
  if (code == StatusCode::OK) {
    checkForWinner();
  }
  return code;
}

StatusCode Game::upgradeSettlement(Player *player, Point *point) {
  StatusCode code = player->buildSettlement(point);
  if (code == StatusCode::OK) {
    checkForWinner();
  }
  return code;
}

bool Game::checkForWinner() {
  for (auto player : players) {
    if (player->calculateVictoryPoints(true) >= victoryPointsToWin) {
      hasEnded = true;
      winner = player;
      return true;
    }
  }
  return false;
}

bool Game::getHasEnded() const { return hasEnded; }

Player *Game::getWinner() const { return winner; }

QVector<DevelopmentCard> &Game::getDevelopmentCards() {
  return developmentCards;
}

StatusCode Game::trade(Player *getter, Player *giver,
                       QVector<ResourceCard> getCards,
                       QVector<ResourceCard> giveCards) {
  if (!getter->hasCards(giveCards) || !giver->hasCards(getCards)) {
    return StatusCode::BadCards;
  }
  getter->removeCards(giveCards);
  getter->addCards(getCards);
  giver->removeCards(getCards);
  giver->addCards(giveCards);
  return StatusCode::OK;
}

StatusCode Game::tradeWithBank(Player *getter, ResourceCard getCard,
                               QVector<ResourceCard> giveCards) {
  if (!getter->hasCards(giveCards)) {
    return StatusCode::BadCards;
  }
  ResourceCard type = giveCards[0];
  for (auto card : giveCards) {
    if (card != type) {
      return StatusCode::BadCards;
    }
  }
  if (giveCards.length() != 4) {
    bool hasHarbor = false;
    QVector<HarborType> types = getter->getConnectedHarborTypes();
    for (auto htype : types) {
      if (Harbor::harborToResourceCard(htype) == type &&
          giveCards.length() == 2) {
        hasHarbor = true;
        break;
      } else if (Harbor::harborToResourceCard(htype) == ResourceCard::None &&
                 giveCards.length() == 3) {
        hasHarbor = true;
        break;
      }
    }
    if (!hasHarbor) {
      return StatusCode::BadHarbor;
    }
  }
  getter->removeCards(giveCards);
  addCards(giveCards);
  getter->addCards({getCard});
  removeCards({getCard});
  return StatusCode::OK;
}

StatusCode Game::activateRobber(Tile *tile, Player *player, Player *victim) {
  if (!board->doesPlayerOwnSettlementAroundTile(tile, victim)) {
    return StatusCode::BadInput;
  }
  for (auto player : board->playersWhoOwnSettlementAroundTile(tile)) {
    player->randomlyRemoveHalfOfCards();
  }
  board->moveRobber(tile);
  if (victim->hasAnyCard()) {
    ResourceCard card = victim->removeRandomCardAndReturnIt();
    player->addCards({card});
  }
  return StatusCode::OK;
}

QPair<int, int> Game::getRoll() {
  lastRoll = {randomNumber(1, 6), randomNumber(1, 6)};
  hasRolled = true;
  return lastRoll;
}

Player *Game::getPlayer(int i) {
  if (i < players.length()) {
    return players[i];
  }
  return nullptr;
}

void Game::checkForLongestRoad() {
  int longestRoadInGame = 0;
  Player *tempLongestRoadOwner = longestRoadOwner;
  for (auto player : players) {
    if (player->getLongestRoadLength() > longestRoadInGame &&
        player->getLongestRoadLength() >= 5) {
      longestRoadInGame = player->getLongestRoadLength();
      tempLongestRoadOwner = player;
    }
  }
  if (tempLongestRoadOwner != longestRoadOwner) {
    longestRoadOwner = tempLongestRoadOwner;
  }
}

void Game::checkForLargestArmy() {
  int largestArmyInGame = 0;
  Player *tempLargestArmyOwner = largestArmyOwner;
  for (auto player : players) {
    if (player->getKnights() > largestArmyInGame && player->getKnights() >= 3) {
      largestArmyInGame = player->getKnights();
      tempLargestArmyOwner = player;
    }
  }
  if (tempLargestArmyOwner != largestArmyOwner) {
    largestArmyOwner = tempLargestArmyOwner;
  }
}

StatusCode Game::playMonoploly(Player *player, ResourceCard card) {
  if (!player->hasDevelopmentCard(DevelopmentCard::Monopoly)) {
    return StatusCode::BadDeck;
  }
  for (auto p : players) {
    if (p == player) {
      continue;
    }
    int numberOfCards = p->howManyOfResource(card);
    if (numberOfCards > 0) {
      QVector<ResourceCard> getCards;
      for (int i = 0; i < numberOfCards; i++) {
        getCards.append(card);
      }
      p->removeCards(getCards);
      player->addCards(getCards);
    }
  }
  player->removeDevelopmentCard(DevelopmentCard::Monopoly);
  return StatusCode::OK;
}

StatusCode Game::playeYearOfPlenty(Player *player, ResourceCard cardOne,
                                   ResourceCard cardTwo) {
  if (!player->hasDevelopmentCard(DevelopmentCard::YearOfPlenty)) {
    return StatusCode::BadDeck;
  } else if (!hasCards({{cardOne, cardTwo}})) {
    return StatusCode::BadDeck;
  }
  player->addCards({cardOne, cardTwo});
  removeCards({cardOne, cardTwo});
  player->removeDevelopmentCard(DevelopmentCard::YearOfPlenty);
  return StatusCode::OK;
}

StatusCode Game::playKnight(Player *player, Tile *tile, Player *victim) {
  if (!player->hasDevelopmentCard(DevelopmentCard::Knight)) {
    return StatusCode::BadDeck;
  }
  activateRobber(tile, player, victim);
  player->increaseKnights();
  player->removeDevelopmentCard(DevelopmentCard::Knight);
  return StatusCode::OK;
}

StatusCode Game::playRoadBuilding(Player *player,
                                  QPair<Point *, Point *> firstRoad,
                                  QPair<Point *, Point *> secondRoad) {
  QVector<ResourceCard> cardsToAdd;
  cardsToAdd.append(roadPrice);
  cardsToAdd.append(roadPrice);
  if (!player->hasDevelopmentCard(DevelopmentCard::RoadBuilding)) {
    return StatusCode::BadDeck;
  }
  if (player->checkRoadLocation(firstRoad.first, firstRoad.second) ==
          StatusCode::OK &&
      player->checkRoadLocation(secondRoad.first, secondRoad.second) ==
          StatusCode::OK) {
    player->addCards(cardsToAdd);
    player->buildRoad(firstRoad.first, firstRoad.second);
    player->buildRoad(secondRoad.first, secondRoad.second);
  } else {
    return StatusCode::WrongPoint;
  }
  player->removeDevelopmentCard(DevelopmentCard::RoadBuilding);
  return StatusCode::OK;
}

QJsonObject Game::toJSON() {
  QJsonObject gameJson;
  QJsonArray playersArray;
  for (auto player : players) {
    QJsonObject p;
    p["name"] = player->getName();
    p["num"] = player->getNum();
    p["color"] = (int)player->getColor();
    p["VPs"] = player->calculateVictoryPoints(true);
    p["roads"] = player->getRoadsCount();
    p["cities"] = player->getCities();
    p["settlements"] = player->getSettlements();
    p["knights"] = player->getKnights();
    QJsonArray playerCards;
    for (auto card : player->getCards()) {
      playerCards.append((int)card);
    }
    p["cards"] = playerCards;
    QJsonArray playerDevCards;
    for (auto card : player->getDevCards()) {
      playerDevCards.append((int)card);
    }
    QJsonArray playerNewDevCards;
    for (auto card : player->getNewDevCards()) {
      playerNewDevCards.append((int)card);
    }
    p["newDevCards"] = playerNewDevCards;
    playersArray.append(p);
  }
  gameJson["players"] = playersArray;
  QJsonObject boardJson;
  QJsonArray robberCoordiantes;
  robberCoordiantes.append(board->getRobber()->getCoordiantes().first);
  robberCoordiantes.append(board->getRobber()->getCoordiantes().second);
  boardJson["robber"] = robberCoordiantes;
  QJsonArray tilesArray;
  for (auto tile : board->getAllTiles()) {
    QJsonObject t;
    QJsonArray tileCoordiantes;
    tileCoordiantes.append(tile->getCoordiantes().first);
    tileCoordiantes.append(tile->getCoordiantes().second);
    t["coordinates"] = tileCoordiantes;
    t["number"] = tile->getNumber();
    t["type"] = (int)tile->getType();
    tilesArray.append(t);
  }
  boardJson["tiles"] = tilesArray;
  QJsonArray pointsArray;
  for (auto point : board->getAllPoints()) {
    QJsonObject p;
    QJsonArray pointCoordiantes;
    pointCoordiantes.append(point->getCoordiantes().first);
    pointCoordiantes.append(point->getCoordiantes().second);
    p["coordinates"] = pointCoordiantes;
    if (point->getPiece() != nullptr) {
      QJsonObject pieceJson;
      pieceJson["owner"] = point->getPiece()->getOwner()->getNum();
      Settlement *settlemnt = dynamic_cast<Settlement *>(point->getPiece());
      if (settlemnt != nullptr) {
        pieceJson["type"] = (int)PieceType::Settlement;
      } else {
        pieceJson["type"] = (int)PieceType::City;
      }
      p["piece"] = pieceJson;
    }
    pointsArray.append(p);
  }
  boardJson["points"] = pointsArray;
  QJsonArray roadsArray;
  for (auto road : board->getRoads()) {
    QJsonObject r;
    QJsonArray roadPoints;
    QJsonArray pointOne;
    QJsonArray pointTwo;
    pointOne.append(road->getStartPoint()->getCoordiantes().first);
    pointOne.append(road->getStartPoint()->getCoordiantes().second);
    pointTwo.append(road->getEndPoint()->getCoordiantes().first);
    pointTwo.append(road->getEndPoint()->getCoordiantes().second);
    roadPoints.append(pointOne);
    roadPoints.append(pointTwo);
    r["points"] = roadPoints;
    r["owner"] = road->getOwner()->getNum();
    roadsArray.append(r);
  }
  boardJson["roads"] = roadsArray;
  QJsonArray harborsArray;
  for (auto harbor : board->getHarbors()) {
    QJsonObject h;
    QJsonArray harborPoints;
    QJsonArray pointOne;
    QJsonArray pointTwo;
    pointOne.append(harbor->getStartPoint()->getCoordiantes().first);
    pointOne.append(harbor->getStartPoint()->getCoordiantes().second);
    pointTwo.append(harbor->getEndPoint()->getCoordiantes().first);
    pointTwo.append(harbor->getEndPoint()->getCoordiantes().second);
    harborPoints.append(pointOne);
    harborPoints.append(pointTwo);
    h["points"] = harborPoints;
    h["type"] = (int)harbor->getType();
    harborsArray.append(h);
  }
  boardJson["harbors"] = harborsArray;
  gameJson["board"] = boardJson;
  if (longestRoadOwner != nullptr) {
    gameJson["longesRoadOwner"] = longestRoadOwner->getNum();
  }
  if (largestArmyOwner != nullptr) {
    gameJson["largestArmyOwner"] = largestArmyOwner->getNum();
  }
  gameJson["devCardsRemaining"] = developmentCards.length();
  if (hasEnded) {
    gameJson["hasEnded"] = true;
    gameJson["winner"] = winner->getNum();
  } else {
    gameJson["hasEnded"] = false;
  }
  QJsonArray boardCards;
  for (auto card : cards) {
    boardCards.append((int)card);
  }
  gameJson["cards"] = boardCards;
  QJsonArray roll;
  roll.append(lastRoll.first);
  roll.append(lastRoll.second);
  gameJson["last roll"] = roll;
  gameJson["current player"] = getCurrentPlayerID();
  gameJson["turn number"] = turnNumber;
  gameJson["start phase"] = startPhase;
  if (tradeHolder != nullptr) {
    QJsonObject trade;
    trade["player id"] = tradeHolder->getGetter()->getNum();
    QJsonArray tradeGet;
    for (auto card : tradeHolder->getGetCards()) {
      tradeGet.append((int)card);
    }
    trade["get cards"] = tradeGet;
    QJsonArray tradeAnswers;
    int i = 0;
    for (auto answer : tradeHolder->getAnswers()) {
      QJsonObject tradeAnswer;
      tradeAnswer["id"] = i++;
      tradeAnswer["player id"] = answer->getGiver()->getNum();
      QJsonArray tradeGive;
      for (auto card : answer->getCards()) {
        tradeGive.append((int)card);
      }
      tradeAnswer["give cards"] = tradeGive;
      tradeAnswers.append(tradeAnswer);
    }
    trade["answers"] = tradeAnswers;
    gameJson["trade"] = trade;
  }
  gameJson["has rolled"] = hasRolled;
  return gameJson;
}

bool Game::hasCards(QVector<ResourceCard> cardsList) {
  QVector<ResourceCard> cardsCopy(cards);
  for (auto card : cardsList) {
    if (!cardsCopy.contains(card)) {
      return false;
    }
    cardsCopy.removeOne(card);
  }
  return true;
}

StatusCode Game::removeCards(QVector<ResourceCard> cardsList) {
  if (!hasCards(cardsList)) {
    return StatusCode::BadDeck;
  }
  for (auto card : cardsList) {
    cards.removeOne(card);
  }
  return StatusCode::OK;
}

StatusCode Game::addCards(QVector<ResourceCard> cardsList) {
  for (auto card : cardsList) {
    cards.append(card);
  }
  return StatusCode::OK;
}

const QVector<Player *> &Game::getPlayers() const { return players; }

void Game::newTrade(Player *player, QVector<ResourceCard> getCards) {
  tradeHolder = new Trade(this, player, getCards);
}

void Game::endTurn() {
  currentPlayerID = (currentPlayerID + 1) % players.length();
  turnNumber++;
  for (auto player : players) {
    player->finishTurn();
  }
  hasRolled = false;
}

QPair<int, int> Game::getLastRoll() const { return lastRoll; }

int Game::getCurrentPlayerID() const { return currentPlayerID; }

int Game::getTurnNumber() const { return turnNumber; }

bool Game::getStartPhase() const { return startPhase; }

void Game::setStartPhase(bool newStartPhase) { startPhase = newStartPhase; }

Trade *Game::getTradeHolder() const { return tradeHolder; }

void Game::setTradeHolder(Trade *newTradeHolder) {
  tradeHolder = newTradeHolder;
}

void Game::acceptTrade(int index) {
  tradeHolder->acceptAnswer(index);
  delete tradeHolder;
  tradeHolder = nullptr;
}
