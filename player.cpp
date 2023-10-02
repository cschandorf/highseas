#include "player.h"

namespace Cylink
{
    /**
     Create a player for battle ship and assign them a board of the size specified by the parameters.
     If no parameters are specified a 10 x 10 board is assumed and created.
     @param boardLength
        The length of board.
     @param boardHeight
        The height of the board.
    */
    Player::Player(int boardLength, int boardHeight)
    : board_(boardLength, boardHeight), ships_()
    {
    }

    /**
     The player is copy constructible.
     @param other
        An existing Player object used to copy initialize the new object.
    */
    Player::Player(const Player& other)
    : board_(other.board_), ships_(other.ships_)
    {
    }

    /**
     Release any allocated resources
    */
    Player::~Player()
    {
    }

    /**
     The player is copy assignable.
     @param other
        An existing Player object used to assign the current objects members.
    */
    Player& Player::operator = (const Player& other)
    {
        if(this != &other)
        {
            board_ = other.board_;
            ships_ = other.ships_;
        }
        return *this;
    }

    bool Player::emplaceVessel(VesselType vtype, int xpos, int ypos, Orientation direction)
    {
        bool result = board_.placeVessel(xpos, ypos, vtype, direction);
        if(result == true)
        {
            ships_.push_back(vtype);
        }
        return result;
    }

    int Player::addVessels(VesselType vtype, int count = 1)
    {
    }

}