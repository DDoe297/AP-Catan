#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QObject>
#include <QVector>
#include "Enums.hpp"
#include "StatusCodes.hpp"
#include "Player_FWD.hpp"
#include "game.hpp"
#include "board.hpp"
#include "Piece_FWD.hpp"
#include "road.hpp"
#include "settelment.hpp"
#include "city.hpp"
#include "harbor.hpp"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QString Name, Game *Game, int Num, Color Color, QObject *parent = nullptr);
    //To String
    //To Map
    const QString &getName(void) const;
    int getNum(void) const;
    Color getColor(void) const;
    StatusCode buildSettelment(Point *point, bool gameStart=false);
    StatusCode buildRoad(Point *startPoint, Point *endPoint, bool gameStart=false);
    bool hasCards(QVector<ResourceCard> cardsList);
    StatusCode removeCards(QVector<ResourceCard> cardsList);
    StatusCode addCards(QVector<ResourceCard> cardsList);
    StatusCode addDevelopmentCard(DevelopmentCard card);
    StatusCode removeDevelopmentCard(DevelopmentCard card);
    bool hasDevelopmentCard(DevelopmentCard card);
    StatusCode finishTurn(void);
    StatusCode checkRoadLocation(Point *startPoint, Point *endPoint);
    void checkLongestRoad(Road *newRoad);
    void checkLongestRoadUtil(Road *road, QVector<Road *> allRoads, int length);
    QVector<HarborType> getConnectedHarborTypes(void);
    QVector<Road *> getRoads(void);
    QVector<Road *> getConnectedRoads(Point *point, QVector<Road *> roads);
    int calculateVictoryPoints(bool includeDevelopmentCards);
private:
    QString name;
    Game *game;
    int num;
    Color color;
    int victoryPoints;
    QVector<ResourceCard> cards;
    QVector<DevelopmentCard> devCards;
    QVector<DevelopmentCard> newDevCards;
    int knights;
    int longestRoadLength;
    int roads;
    int cities;
    int settelments;
};

#endif // PLAYER_HPP
