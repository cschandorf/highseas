#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <iostream>
#include <vector>
#include "vessel.h"

#define GB_BOARD_SIZE 10
#define GB_NO_VESSEL -1

/**
 * @namespace Cylink
 * General project namespace
 */
namespace Cylink
{
    /**
     Notes:
     Build a dynamic GameBoard of specified size.
     Once a ship is hit, hitting the same spot does not count
    */
    class GameBoard
    {
    public: 
        /**
         @enum StrikeType
         Indicates the type of strike attempted.
        */
        enum class StrikeType
        {
            STYPE_NONE,
            STYPE_FAIL,
            STYPE_HIT
        };

        /**
         @enum StrikeResult
         Information about the result of launching a strike on a specific board location.
        */
        enum class StrikeResult
        {
            STRIKE_INVALID,         //Invalid strike location
            STRIKE_PREVIOUS,        //Previous strike location
            STRIKE_MISS,            //Miss strike location
            STRIKE_HIT,             //Hit strike location
            STRIKE_DESTROYED        //Hit destroyed the vessel
        };

        /**
         @enum VDirection
         Specify the direction of a Vessel on a grid.
         VERTICAL means the long side of the vessel (length) goes top to bottom.
         HORIZONTAL means the long side of the vessel (length) goes left to right.
        */
        enum class VDirection
        {
            VERTICAL,
            HORIZONTAL
        };    

        /**
         @struct BoardData
         A structure for data that is stored at individual board locations.
         Declaration shows default field values.
        */
        struct BoardData
        {
            int vesselId = GB_NO_VESSEL;
            StrikeType received = StrikeType::STYPE_NONE;
            StrikeType launched = StrikeType::STYPE_NONE;
            VDirection direction = VDirection::HORIZONTAL;
        };

        /**
         @struct VBorder
         Specify the enclosing border rectangle for a vessel.
         A border is always relative to the top left coordinates.
         Declaration shows default field values.
        */
        struct VBorder
        {
            int topX;
            int topY;
            int lowX;
            int lowY;

            VBorder(int tx = -1, int ty = -1, int lx = -1, int ly = -1)
            {
                topX = tx;
                topY = ty;
                lowX = lx;
                lowY = ly;
            }
        };
        
    public:
        GameBoard(int length = GB_BOARD_SIZE, int width = GB_BOARD_SIZE);
        GameBoard(const GameBoard& other);
        ~GameBoard();
        GameBoard& operator =(const GameBoard& other);

        bool emplaceVessel(VBorder& vrect, Vessel::VType vtype, VDirection vdir);
        bool addVessel(Vessel::VType vtype);
        void logLaunchedAttack(VBorder& vrect, StrikeResult sresult);
        StrikeResult logReceivedAttack(VBorder& vrect);

        static int randomNumber(int start, int end, bool seedFlag = false);
        friend std::ostream& operator<<(std::ostream& os, const GameBoard& gb);
        
    private:
        int findOpenPosition(Vessel::VType vtype, VDirection vdir);    
        std::vector<int> getPositionMap(VBorder& vrect, Vessel::VType vtype, VDirection vdir) const; 
        int isAreaOccupied(std::vector<int>& vmap) const;
        bool isValidBorder(VBorder& vrect) const;
        bool isValidTopXY(VBorder& vrect) const;
        bool isValidLowXY(VBorder& vrect) const;

        int coordinateIndex(VBorder& vrect, bool topFlag = true) const;
        std::pair<int, int> coordinates(int index) const;
        
        static void border(VBorder& vrect, Vessel::VType vtype, VDirection vdir);

    private:
        int lengthOfBoard_;
        int widthOfBoard_;
        std::vector<BoardData> board_;  /**< Gameboard as a one-dimensional vector */
        std::vector<Vessel> vessels_;   /**< The various vessels on the GameBoard */
    };
}

#endif