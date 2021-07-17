#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "game.hpp"

int main(void) {
  QFile file("DataExample.json");
  QVector<initialPlayerData> d;
  d.append({"Danny", Color::Blue});
  d.append({"Hadi", Color::Orange});
  d.append({"Fatemeh", Color::Red});
  d.append({"Asghar", Color::White});
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Game game = Game(d);
    game.addSettlement(game.getPlayer(0), game.getBoard()->getPoint(3, 2),
                       true);
    game.addSettlement(game.getPlayer(1), game.getBoard()->getPoint(5, 1),
                       true);
    game.addSettlement(game.getPlayer(2), game.getBoard()->getPoint(1, 1),
                       true);
    game.addSettlement(game.getPlayer(3), game.getBoard()->getPoint(2, 4),
                       true);
    game.addRoad(game.getPlayer(0), game.getBoard()->getPoint(3, 2),
                 game.getBoard()->getPoint(3, 1), true);
    game.addRoad(game.getPlayer(1), game.getBoard()->getPoint(5, 1),
                 game.getBoard()->getPoint(5, 2), true);
    game.addRoad(game.getPlayer(2), game.getBoard()->getPoint(1, 1),
                 game.getBoard()->getPoint(1, 0), true);
    game.addRoad(game.getPlayer(2), game.getBoard()->getPoint(2, 5),
                 game.getBoard()->getPoint(2, 4), true);
    game.getBoard()->addResource(5);
    QJsonDocument save(game.toJSON());
    file.write(save.toJson());
  }
  file.close();
}
