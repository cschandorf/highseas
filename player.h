#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "gameboard.h"

#define BOARD_SIZE 10

/**
 * @namespace Cylink
 * General project namespace
 */
namespace Cylink
{
    /**
     Models a player within the highseas game.
    */
    class Player
    {
    public:
        Player(int boardLength = BOARD_SIZE, int boardHeight = BOARD_SIZE);
        Player(const Player& other);
        ~Player();

        Player& operator = (const Player& other);
        int setupBoard(const std::vector<Vessel::VType>& vessels);
        //GameBoard::StrikeResult launchAttack(Player& other, int xpos, int ypos);
        void launchAttack(Player& other, int xCord, int yCord);
        bool hasVessels();
        std::pair<int, int> suggestFirePosition();
    
    private:
        int receiveAttack(int xpos, int ypos);

    private:
        GameBoard board_; 
    };
}

#endif