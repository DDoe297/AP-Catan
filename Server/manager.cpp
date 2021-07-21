#include "manager.hpp"

void Manager::gameTurn() {
  if (!game->getHasEnded()) {
    playerChoice();
    game->checkForLongestRoad();
    game->checkForLargestArmy();
    game->checkForWinner();
    QJsonDocument gameJson(game->toJSON());
    emit server->write(gameJson.toJson());
    if (game->getTurnNumber() == 8) {
      game->setStartPhase(false);
    }
  }
}

void Manager::playerChoice() {
  if (!server->getCommandQueue().isEmpty()) {
    QJsonObject clientMessage = server->getCommandQueue().dequeue();
    QString command = clientMessage["command"].toString();
    if (clientMessage["id"].toInt() == game->getCurrentPlayerID()) {
      if (command == "build road") {
        Player *player = game->getPlayer(game->getCurrentPlayerID());
        QJsonArray coordinates = clientMessage["coordinates"].toArray();
        QJsonArray pOne = coordinates[0].toArray();
        QJsonArray pTwo = coordinates[1].toArray();
        Point *pointOne =
            game->getBoard()->getPoint(pOne[0].toInt(), pOne[1].toInt());
        Point *pointTwo =
            game->getBoard()->getPoint(pTwo[0].toInt(), pTwo[1].toInt());
        game->addRoad(player, pointOne, pointTwo, game->getStartPhase());
      } else if (command == "build settlement") {
        Player *player = game->getPlayer(game->getCurrentPlayerID());
        QJsonArray coordinates = clientMessage["coordinates"].toArray();
        Point *point = game->getBoard()->getPoint(coordinates[0].toInt(),
                                                  coordinates[1].toInt());
        game->addSettlement(player, point, game->getStartPhase());
      } else if (command == "end turn") {
        game->endTurn();
      }
      if (!game->getStartPhase()) {
        if (command == "roll" && !game->getHasRolled()) {
          QPair<int, int> roll = game->getRoll();
          game->getBoard()->addResource(roll.first + roll.second);
        } else if ((command == "activate robber" &&
                    game->getLastRoll().first + game->getLastRoll().second ==
                        7)) {
          QJsonArray coordinates = clientMessage["coordinates"].toArray();
          int victimId = clientMessage["victim"].toInt();
          game->activateRobber(
              game->getBoard()->getTile(coordinates[0].toInt(),
                                        coordinates[1].toInt()),
              game->getPlayer(game->getCurrentPlayerID()),
              game->getPlayer(victimId));
        } else if (command == "buy devCard") {
          game->getPlayer(game->getCurrentPlayerID())->buyDevelopmentCard();
        } else if (command == "upgrade") {
          Player *player = game->getPlayer(game->getCurrentPlayerID());
          QJsonArray coordinates = clientMessage["coordinates"].toArray();
          Point *point = game->getBoard()->getPoint(coordinates[0].toInt(),
                                                    coordinates[1].toInt());
          game->upgradeSettlement(player, point);
        } else if (command == "trade") {
          QJsonArray resourcesJson = clientMessage["get cards"].toArray();
          Player *player = game->getPlayer(game->getCurrentPlayerID());
          QVector<ResourceCard> resources;
          for (auto resource : resourcesJson) {
            resources.append((ResourceCard)resource.toInt());
          }
          game->newTrade(player, resources);
        } else if (command == "accept trade") {
          game->getTradeHolder()->acceptAnswer(
              clientMessage["tradeID"].toInt());
        } else if (command == "trade bank") {
          QJsonArray resourcesJson = clientMessage["give cards"].toArray();
          int getCard = clientMessage["get card"].toInt();
          Player *player = game->getPlayer(game->getCurrentPlayerID());
          QVector<ResourceCard> resources;
          for (auto resource : resourcesJson) {
            resources.append((ResourceCard)resource.toInt());
          }
          game->tradeWithBank(player, (ResourceCard)getCard, resources);
        } else if (command == "knight" && !game->getHasUsedDevCard()) {
          QJsonArray coordinates = clientMessage["coordinates"].toArray();
          int victimId = clientMessage["victim"].toInt();
          game->playKnight(game->getPlayer(game->getCurrentPlayerID()),
                           game->getBoard()->getTile(coordinates[0].toInt(),
                                                     coordinates[1].toInt()),
                           game->getPlayer(victimId));
        } else if (command == "monopoly" && !game->getHasUsedDevCard()) {
          ResourceCard card = (ResourceCard)clientMessage["card"].toInt();
          Player *player = game->getPlayer(game->getCurrentPlayerID());
          game->playMonoploly(player, card);
        } else if (command == "year of plenty" && !game->getHasUsedDevCard()) {
          ResourceCard cardOne =
              (ResourceCard)clientMessage["card one"].toInt();
          ResourceCard cardTwo =
              (ResourceCard)clientMessage["card two"].toInt();
          Player *player = game->getPlayer(game->getCurrentPlayerID());
          game->playeYearOfPlenty(player, cardOne, cardTwo);
        } else if (command == "road building" && !game->getHasUsedDevCard()) {
          Player *player = game->getPlayer(game->getCurrentPlayerID());
          QJsonArray coordinates = clientMessage["coordinates"].toArray();
          QJsonArray cOne = coordinates[0].toArray();
          QJsonArray cTwo = coordinates[1].toArray();
          QJsonArray pOne = cOne[0].toArray();
          QJsonArray pTwo = cOne[1].toArray();
          QJsonArray pThree = cTwo[0].toArray();
          QJsonArray pFour = cTwo[1].toArray();
          Point *pointOne =
              game->getBoard()->getPoint(pOne[0].toInt(), pOne[1].toInt());
          Point *pointTwo =
              game->getBoard()->getPoint(pTwo[0].toInt(), pTwo[1].toInt());
          Point *pointThree =
              game->getBoard()->getPoint(pThree[0].toInt(), pThree[1].toInt());
          Point *pointFour =
              game->getBoard()->getPoint(pFour[0].toInt(), pFour[1].toInt());
          game->playRoadBuilding(player, {pointOne, pointTwo},
                                 {pointThree, pointFour});
        }
      }
    }
    if (game->getTradeHolder() != nullptr) {
      if (command == "trade answer") {
        QJsonArray resourcesJson = clientMessage["give cards"].toArray();
        Player *giver = game->getPlayer(clientMessage["id"].toInt());
        QVector<ResourceCard> resources;
        for (auto resource : resourcesJson) {
          resources.append((ResourceCard)resource.toInt());
        }
        game->getTradeHolder()->addAnswer(giver, resources);
      }
    }
  }
}

const QVector<QString> &Manager::getPlayerNames() const { return playerNames; }

void Manager::startServer() { server->startServer(); }

void Manager::prepareGame() {
  game = new Game(playerNames);
  QJsonDocument gameJson(game->toJSON());
  emit server->write(gameJson.toJson());
}

void Manager::addToPlayerNames(QString name) { playerNames.append(name); }

Manager::Manager(void) { server = new Server(this); }

Manager::~Manager()
{
    delete game;
    delete server;
}
