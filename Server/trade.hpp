#ifndef TRADE_HPP
#define TRADE_HPP

#include <QObject>
#include <QVector>

#include "Enums.hpp"
#include "Game_FWD.hpp"
#include "TradeAnswer_FWD.hpp"
#include "Trade_FWD.hpp"
#include "game.hpp"
#include "player.hpp"
#include "tradeanswer.hpp"

class Trade : public QObject {
  Q_OBJECT
 public:
  explicit Trade(Game *Game, Player *Getter, QVector<ResourceCard> Cards,
                 QObject *parent = nullptr);
  void addCards(QVector<ResourceCard> cardsList);
  void addAnswer(Player *giver, QVector<ResourceCard> giveCards);
  const QVector<ResourceCard> &getGetCards() const;
  void setGetCards(const QVector<ResourceCard> &newGetCards);
  const QVector<TradeAnswer *> &getAnswers() const;
  void acceptAnswer(int index);
  Player *getGetter() const;

 private:
  Game *game;
  Player *getter;
  QVector<ResourceCard> getCards;
  QVector<TradeAnswer *> answers;
};

#endif  // TRADE_HPP
