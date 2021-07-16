#ifndef STATUSCODES_HPP
#define STATUSCODES_HPP

enum class StatusCode {
  OK = 2,
  BadCards = 3,
  PieceBlock = 4,
  WrongPoint = 5,
  NotConnected = 6,
  IsolatedPiece = 7,
  BadHarbor = 8,
  PieceDosentExist = 9,
  WrongOwner = 10,
  CantUpgradeCity = 11,
  BadDeck = 12,
  BadInput = 13,
  OutOfPieces = 14
};

#endif  // STATUSCODES_HPP
