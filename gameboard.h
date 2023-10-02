#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <iostream>
#include <vector>
#include "vessel.h"

#define BOARD_SIZE 10

/**
 * @namespace Cylink
 * General project namespace
 */
namespace Cylink
{
    /**
     @enum Orientation
     Specify the orientation of a ship on the board.
    */
    enum class Orientation
    {
        VERTICAL,
        HORIZONTAL
    };

    /**
     @enum StrikeInfo
     Indicates whether or not a strike has been attempted at a specific location and the outcome.
    */
    enum class StrikeInfo
    {
        NONE,
        FAILED,
        HIT
    };

    /**
     @struct BoardInfo
    */
    struct BoardInfo
    {
        int shipLocation;
        StrikeInfo strikeStatus;
    };

    /**
     Notes:
     Build a dynamic GameBoard of specified size.
     Once a ship is hit, hitting the same spot does not count
    */
    class GameBoard
    {
    public:
        GameBoard(int length = BOARD_SIZE, int height = BOARD_SIZE);
        GameBoard(const GameBoard& other);
        ~GameBoard();
        GameBoard& operator =(const GameBoard& other);

        bool emplaceVessel(int xpos, int ypos, VesselType vtype, Orientation direction);
        int getCoordinateIndex(int xpos, int ypos) const;
        std::pair<int, int> getValidPosition(VesselType vtype, Orientation direction);

        friend std::ostream& operator<<(std::ostream& os, const GameBoard& gb);
        
    private:
        bool fitVessel(int xpos, int ypos, VesselType vtype, Orientation direction);
        std::vector<int> getPositionMap(int xpos, int ypos, VesselType vtype, Orientation direction); 
        static int randomNumber(int start, int end);

    private:
        int lengthOfBoard_;
        int heightOfBoard_;
        std::vector<BoardInfo> board_;
        std::vector<Vessel> warships_;
    };
}

#endif