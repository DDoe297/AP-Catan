#include <QCoreApplication>
#include "game.hpp"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  Game game=Game();
  game.addSettlement(game.getPlayer(0),game.getBoard()->getPoint(0, 0),true);
  game.addRoad(game.getPlayer(0), game.getBoard()->getPoint(0, 0),game.getBoard()->getPoint(0, 1), true);
  for(auto tile:game.getBoard()->getAllTiles()){
   qDebug()<<tile->getCoordiantes()<<"\n";
  }
  qDebug()<<game.getPlayer(0)->getRoads()[0]->getStartPoint();
}
