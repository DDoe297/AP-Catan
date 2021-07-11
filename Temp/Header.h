#pragma once
#include <iostream>
#include <map>

class Game {
 private:
  Player players;
  Player currentPlayer;
  Board board;
  std::map<enum Resources, int> resources;
  int lastRoll;
  Player *biggestArmyOwner;
  Player *longestRoadOwner;
  int currentTurn;

 public:
  // getter
  int getlastRoll() { return lastRoll; };
  int getcurrentTurn() { return currentTurn; };

  void start();
  void end();
  void roll();
  void stealablePlayers();
  void stealFromPlayers();
  void buyDevCard();
  void playDevCard();
  void buyRoad();
  void buyBridge();
  void buySettelment();
  void buyCity();
  void endTurn();
};

class Board {
 private:
  Tile tiles;
  Port ports;
  Piece pieces;

 public:
  static void generateBoard();
  void canPlacePiece();
  void placePiece();
  void movePiece();
  void removePiece();
  void moveRobber();
  void checkTouchingTiles(int coord);
};

class Player {
 private:
  std::string name;
  std::string color;
  Piece _pieces;
  Resources _resources;
  // Card cards;
 public:
  void trade();
  void buyRoad();
  void buyBridge();
  void buySettelment();
  void buyCity();
  void buyDevCard();
  void moveRobber();
  void roll();
};

class Trade {
 private:
  Player getter;
  Resources get;
  TradeAnswer answers;

 public:
  void addGet();
};

class TradeAnswer {
 private:
  Player giver;
  Resources give;

 public:
  void addGive();
  void accept();
};

class Tile {
 private:
  int number;
  bool hasRobber;
  int coordinates;
  Terrains terrain;

 public:
};

class Port {
 private:
  std::string type;

 public:
};

class Piece {
 private:
  Player *owner;
  std::map<enum Resources, int> price;

 public:
};

class Card {
 private:
 public:
  virtual void action() = 0;
};

class City : public Piece {
 private:
 public:
};

class Settelment : public Piece {
 private:
 public:
};

class Road : public Piece {
 private:
 public:
};

class Bridge : public Piece {
 private:
 public:
};

class Knight : public Card {
 private:
 public:
  void action();
};

class Progress : public Card {
 private:
 public:
  void action();
};

class Victory : public Card {
 private:
 public:
  void action();
};

enum Terrains { Hill, Forest, Mountain, Field, Pasture, Dessert, Sea };

enum Resources {

};