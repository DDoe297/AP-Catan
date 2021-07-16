#include "tradeanswer.hpp"

TradeAnswer::TradeAnswer(Player *Giver, QVector<ResourceCard> Cards,
                         QObject *parent)
    : QObject(parent) {
  giver = Giver;
  cards = Cards;
}

void TradeAnswer::addCards(QVector<ResourceCard> cardsList) {
  for (auto card : cardsList) {
    cards.append(card);
  }
}

const QVector<ResourceCard> &TradeAnswer::getCards() const { return cards; }

void TradeAnswer::setCards(const QVector<ResourceCard> &newCards) {
  cards = newCards;
}

Player *TradeAnswer::getGiver() const { return giver; }
