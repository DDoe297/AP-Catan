#include "piece.hpp"

Piece::Piece(Player *Owner, PieceType Type, QObject *parent) : QObject(parent) {
  owner = Owner;
  type = Type;
}

Player *Piece::getOwner() const { return owner; }
