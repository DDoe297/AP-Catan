#ifndef TRADEANSWER_HPP
#define TRADEANSWER_HPP

#include <QObject>
#include <QVector>

#include "Enums.hpp"
#include "TradeAnswer_FWD.hpp"
#include "Trade_FWD.hpp"
#include "player.hpp"
#include "trade.hpp"

class TradeAnswer : public QObject {
  Q_OBJECT
 public:
  explicit TradeAnswer(Player *Giver, QVector<ResourceCard> Cards,
                       QObject *parent = nullptr);
  void addCards(QVector<ResourceCard> cards);
  const QVector<ResourceCard> &getCards() const;
  void setCards(const QVector<ResourceCard> &newCards);
  Player *getGiver() const;

 private:
  Player *giver;
  QVector<ResourceCard> cards;
};

#endif  // TRADEANSWER_HPP
