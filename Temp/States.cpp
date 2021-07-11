#include "States.hpp"
bool GameStateNotInGame::isInGame(void) { return false; }
bool GameStateNotInGameMoveRobber::canMoveRobber(void) { return true; }
bool GameStateInGame::isInGame(void) { return true; }
bool GameStateInGame::isInPreGame(void) { return false; }
bool GameStateInGame::nextPlayer(void) {
  //     return game->players[(game->currentTurn + 1) % (game->players.size())];
}
void beginTurn(void) {
  // game->setState(GameStateBeginTurn(game));
}