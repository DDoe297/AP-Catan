class Game;
class GameState {
 public:
  GameState(Game *game_) { game = game_; }

 protected:
  Game *game;
};
class GameStateNotInGame : public GameState {
 public:
  virtual bool isInGame(void);
};

class GameStateNotInGameMoveRobber : public GameStateNotInGame {
 public:
  virtual bool canMoveRobber(void);
};
class GameStateInGame : public GameState {
 public:
  virtual bool isInGame(void);
  virtual bool isInPreGame(void);
  virtual bool nextPlayer(void);
  virtual void beginTurn(void);
  virtual bool hasRolled(void);
  virtual bool canRoll(void);
  virtual bool canMoveRobber(void);
  virtual bool canSteal(void);
  virtual bool canBuyRoad(void);
  virtual bool canBuySettelment(void);
  virtual bool canBuyCity(void);
  virtual bool canPlaveRoad(void);
  virtual bool canPlaceSettelment(void);
  virtual bool canPlaceCity(void);
  virtual bool canBuyDevCard(void);
  virtual bool canTrade(void);
  virtual bool canPlayKnight(void);
  virtual bool canPlayMonopoly(void);
  virtual bool canPlayYearOfPlenty(void);
  virtual bool canPlayRoadBuilder(void);
  virtual bool canPlayVictoryPoint(void);
  virtual bool canEndTurn(void) = 0;
};
class GameStatePreGame : public GameStateInGame {
 public:
};