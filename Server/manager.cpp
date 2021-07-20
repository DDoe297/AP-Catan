#include "manager.h"
#include "game.hpp"

Manager::Manager()
{

}

void Manager::preparingGame(){

    //rang adad esm

    //build resource 2bar
    for(int i=0;i<2;i++){
        for(int j=0;j<4;j++){
            _game.addSettlement(_game.getPlayer(j), _game.getBoard()->getPoint(2, 4),true);
            _game.addRoad(_game.getPlayer(j), _game.getBoard()->getPoint(3, 2),_game.getBoard()->getPoint(3, 1), true);
        }
    }
    startGame();
}

void Manager::startGame(){

    while(!_game.getHasEnded()){
        playerTurn();
        _game.checkForLongestRoad();
        _game.checkForLargestArmy();
        _game.checkForWinner();
    }
    //send winner
}

void Manager::playerTurn(){
    //roll
    int roll=_game.getRoll();
    //robber ya resource
    //if(roll==7)
      //  _game.activateRobber();
    //else _game.getBoard()->addResource(roll);



    /*while(true){
        switch () {
        case "trade":
            //_game.trade();
            break;
        case "tradeb":
        //_game.tradeWithBank();
            break;
        case "buildroad":
        //build
            break;
        case "buildsett":
        //build
            break;
        case "upgrade":
        //build
            break;
        case "buycard":
        //build
            break;
        case "playcard":
        //play cards
            break;
        }
    }*/
    //player++
}
