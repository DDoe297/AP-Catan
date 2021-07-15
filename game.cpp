#include "game.hpp"

Game::Game(int numberOfPlayers, int VictoryPointsToWin, QObject *parent)
    : QObject(parent) {
  board = new Board(this);
  for (int i = 0; i < numberOfPlayers; i++) {
    Player *newPlayer = new Player(QString::number(i), this, i, (Color)i);
    // Get input for name and color
    players.append(newPlayer);
  }
  victoryPointsToWin = VictoryPointsToWin;
  developmentCards = QVector<DevelopmentCard>(devCards);
  shuffle(developmentCards);
  hasEnded = false;
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

StatusCode Game::addSettelment(Player *player, Point *point, bool gameStart) {
  StatusCode code = player->buildSettelment(point, gameStart);
  if (code == StatusCode::OK) {
    checkForWinner();
  }
  return code;
}

StatusCode Game::upgradeSettelment(Player *player, Point *point) {
  StatusCode code = player->buildSettelment(point);
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
  getter->addCards({getCard});
  return StatusCode::OK;
}

StatusCode Game::activateRobber(Tile *tile, Player *player, Player *victim) {
  if (!board->doesPlayerOwnSettelmentAroundTile(tile, victim)) {
    return StatusCode::BadInput;
  }
  for (auto player : board->playersWhoOwnSettelmentAroundTile(tile)) {
    player->randomlyRemoveHalfOfCards();
  }
  board->moveRobber(tile);
  if (victim->hasAnyCard()) {
    ResourceCard card = victim->removeRandomCardAndReturnIt();
    player->addCards({card});
  }
  return StatusCode::OK;
}

int Game::getRoll() { return randomNumber(1, 6) + randomNumber(1, 6); }

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
