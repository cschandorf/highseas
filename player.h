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
    class Player
    {
    public:
        Player(int boardLength = BOARD_SIZE, int boardHeight = BOARD_SIZE);
        Player(const Player& other);
        ~Player();

        Player& operator = (const Player& other);

        bool emplaceVessel(VesselType vtype, int xpos, int ypos, Orientation direction);
        int addVessels(VesselType vtype, int count = 1);

    private:
        GameBoard board_;   
        std::vector<Vessel> ships_;     
    };
}

#endif