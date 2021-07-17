#include "Enums.hpp"
const QVector<ResourceCard> gameDeck(void) {
  QVector<ResourceCard> deck;
  for (int i = 0; i < 19; i++) {
    deck.append(ResourceCard::Lumber);
    deck.append(ResourceCard::Brick);
    deck.append(ResourceCard::Ore);
    deck.append(ResourceCard::Wool);
    deck.append(ResourceCard::Grain);
  }
  return deck;
}
