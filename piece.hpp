#ifndef PIECE_HPP
#define PIECE_HPP

#include <QObject>
#include "Enums.hpp"
#include "Piece_FWD.hpp"
#include "Player_FWD.hpp"

class Piece : public QObject
{
    Q_OBJECT
public:
    explicit Piece(Player *Owner,PieceType Type ,QObject *parent = nullptr);
    Player *getOwner() const;

protected:
    Player *owner;
    PieceType type;

};

#endif // PIECE_HPP
