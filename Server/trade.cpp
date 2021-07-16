#include "trade.hpp"

Trade::Trade(Game *Game, Player *Getter, QVector<ResourceCard> Cards,
             QObject *parent)
    : QObject(parent) {
  game = Game;
  getter = Getter;
  getCards = Cards;
}

void Trade::addCards(QVector<ResourceCard> cardsList) {
  for (auto card : cardsList) {
    getCards.append(card);
  }
}

void Trade::addAnswer(Player *giver, QVector<ResourceCard> giveCards) {
  TradeAnswer *ans = new TradeAnswer(giver, giveCards);
  answers.append(ans);
}

const QVector<ResourceCard> &Trade::getGetCards() const { return getCards; }

void Trade::setGetCards(const QVector<ResourceCard> &newGetCards) {
  getCards = newGetCards;
}

const QVector<TradeAnswer *> &Trade::getAnswers() const { return answers; }

void Trade::acceptAnswer(int index) {
  Player *giver = answers[index]->getGiver();
  QVector<ResourceCard> giveCards = answers[index]->getCards();
  game->trade(getter, giver, getCards, giveCards);
}
