#ifndef TRADEANSWER_HPP
#define TRADEANSWER_HPP

#include <QObject>
#include <QVector>

#include "Enums.hpp"
#include "player.hpp"

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
