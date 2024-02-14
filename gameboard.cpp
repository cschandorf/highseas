#include <tuple>
#include <cstdlib>

#include "gameboard.h"
#include "error.h"
#include "utils.h"

#define BOARD_SQUARE 5
#define GB_ARG_FILTER "INVALID_ARG"
#define GB_ARG_ERROR "CY0000"

#define GB_INVALID_POSITION -1

namespace Cylink
{
    /**
     Create a game board with the given dimensions. The board doesn't have to be symetric.
     Negative of zero argument values for length or width will result in an exception.
     @param length
        The length of the board. Defaults to GB_BOARD_SIZE if unspecified. Value must be greater than zero.
     @param width
        The width of the board. Defaults to GB_BOARD_SIZE if unspecified. Value must be greater than zero.
    */
    GameBoard::GameBoard(int length, int width)
        : lengthOfBoard_(length), widthOfBoard_(width), board_(), vessels_()
    {
        if(lengthOfBoard_ <= 0 || widthOfBoard_ <= 0)
        {
            Error argError("Board length and height must be greater than zero.", GB_ARG_ERROR, GB_ARG_FILTER, __FILE__, __LINE__);
            throw argError;
        }

        /* Seed the random number generator */
        randomNumber(0, 1, true);
    }

    /**
     The game board is copy constructible.
     @param other
        An existing GameBoard object used to copy initialize the new object.
    */
    GameBoard::GameBoard(const GameBoard& other)
        : lengthOfBoard_(other.lengthOfBoard_), widthOfBoard_(other.widthOfBoard_), board_(other.board_), vessels_(other.vessels_)
    {
        /* Seed the random number generator */
        randomNumber(0, 1, true);
    }

    /**
     Release any allocated resources
    */
    GameBoard::~GameBoard()
    {
    }

    /**
     The game board is copy assignable.
     @param other
        An existing GameBoard object used to assign the current objects members.
    */
    GameBoard& GameBoard::operator =(const GameBoard& other)
    {
        if(this != &other)
        {
            lengthOfBoard_ = other.lengthOfBoard_;
            widthOfBoard_ = other.widthOfBoard_;
            /* Copy the contents between vectors below */
            board_ = other.board_;
            vessels_ = other.vessels_;
        }
        return *this;
    }

    /**
     Place a vessel on the gameboard at a random location with a random orientation.
     The function should generally succeed unless there is no place on the gameboard to fit a vessel
     of the given size.
     @param vtype
        The type of vessel to be placed on the board.
     @return
        True if the vessel is successfully placed, false otherwise.
    */
    bool GameBoard::addVessel(Vessel::VType vtype)
    {
        bool result = false;
        int boardPos = GB_INVALID_POSITION;
        VDirection vdir;

        /* Attempt to find an open position for the given vessel type. */
        if (randomNumber(0, 1, false) == 1)
        {
            vdir = VDirection::VERTICAL;
            boardPos = findOpenPosition(vtype, vdir);
            if (boardPos == GB_INVALID_POSITION)
            {
                vdir = VDirection::HORIZONTAL;
                boardPos = findOpenPosition(vtype, vdir);
            }
        }
        else
        {
            vdir = VDirection::HORIZONTAL;
            boardPos = findOpenPosition(vtype, vdir);
            if (boardPos == GB_INVALID_POSITION)
            {
                vdir = VDirection::VERTICAL;
                boardPos = findOpenPosition(vtype, vdir);
            }
        }

        /* Place vessel on board if a good position is found. */
        if (boardPos != GB_INVALID_POSITION)
        {
            VBorder vrect;
            std::tie(vrect.topX, vrect.topY) = coordinates(boardPos);
            result = emplaceVessel(vrect, vtype, vdir);
        }

        return result;
    }

    /**
     Put a vessel on the game board if it's determined to meet the required criteria.
     Vessels may not overlap on the same game board, though another player may have a vessel in the same position on their board.
     The vessels must fit at the specified location on the game board given it's start position, orientation and size.
     @param vrect
        The enclosing border rectangle for the specified vessel type.
        Caller must provide values for border members (topX, topY)
     @param vtype
        The type of vessel being placed on the game board.
     @param vdir
        The direction to place the vessel on the game board.
     @return
        Return true if the indicated vessel can be fit on the gameboard at the suggested location.        
    */
    bool GameBoard::emplaceVessel(VBorder& vrect, Vessel::VType vtype, VDirection vdir)
    {
        bool result = false;

        //Get indexes occupied by the vessel
        std::vector<int> vmap = getPositionMap(vrect, vtype, vdir); 

        //A valid border implies a valid map was returned so...
        if(isValidBorder(vrect) == true && isAreaOccupied(vmap) == GB_INVALID_POSITION)
        {
            //Add vessel to warship list
            vessels_.push_back(Vessel(vtype));
            int vesselId = vessels_.size()-1;

            //Update board with vessel position
            int mSize = vmap.size();
            for(int idx=0; idx<mSize; idx++)
            {
                board_[vmap[idx]].vesselId = vesselId;
            }
            //set the result
            result = true;
        }
        return result;
    }

    /**
     Records an attack on a specified location on the gameboard and returns the result of that attack.
     This record saves information about an oppoenents attack. To save information about an attack launched against an opponent
     call the @logLaunchedAttack() function.
     The result of the attack is one the StrikeResult enumeration.
     @param vrect
        Pass the coordinates to be attacked as the (topX, topY) coordinates of the vrect.
     @return 
        A StrikeResult indicating the results of the attack.
    */
    GameBoard::StrikeResult GameBoard::logReceivedAttack(VBorder& vrect)
    {
        StrikeResult result = StrikeResult::STRIKE_INVALID;

        //Return if the location specified is invalid
        if (!isValidTopXY(vrect))
            return result;

        //Determine whether the location has been hit before or not
        int index = coordinateIndex(vrect, true);
        switch (board_[index].received)
        {
        case StrikeType::STYPE_NONE:
            if (board_[index].vesselId == GB_NO_VESSEL)
            {
                board_[index].received = StrikeType::STYPE_FAIL;
                result = StrikeResult::STRIKE_MISS;
            }
            else
            {
                board_[index].received = StrikeType::STYPE_HIT;
                result = StrikeResult::STRIKE_HIT;
            }
            break;

        case StrikeType::STYPE_FAIL:
            result = StrikeResult::STRIKE_MISS;
            break;

        case StrikeType::STYPE_HIT:
            result = StrikeResult::STRIKE_PREVIOUS;
            break;            
        }

        //If this was a hit, determine whether vessel is destroyed.
        if (result == StrikeResult::STRIKE_HIT)
        {
            if (vessels_[board_[index].vesselId].takeHit(1) == 0)
                result = StrikeResult::STRIKE_DESTROYED;
        }

        return result;
    }

    /**
     Records an attack launched against an opponent at a specified location.
     This record saves information about where the opponent has been attacked. 
     To save information about an attack launched by the opponent, call the @logReceivedAttack() function.
     @param vrect
        Pass the coordinates to be attacked as the (topX, topY) coordinates of the vrect.
    */
    void GameBoard::logLaunchedAttack(VBorder& vrect, StrikeResult sresult)
    {
        //Return if the location specified is invalid
        if (!isValidTopXY(vrect))
            return;

        //Determine whether the location has been hit before or not
        int index = coordinateIndex(vrect, true);
        switch(sresult)
        {
        case StrikeResult::STRIKE_INVALID:
        case StrikeResult::STRIKE_PREVIOUS:
        case StrikeResult::STRIKE_MISS:
            board_[index].launched =  StrikeType::STYPE_FAIL;
            break;
        case StrikeResult::STRIKE_HIT:
        case StrikeResult::STRIKE_DESTROYED:
            board_[index].launched = StrikeType::STYPE_HIT;
            break;
        }
    }

    /**
     Find a random position on the board where the specified vessel can be placed.
     @param vtype
        Is the type of vessel to be placed on the board.
     @param vdir
        Is the orientation of the vessel when placed on board.
     @return
        An integer corresponding to the identified open position or GB_INVALID_POSITION if nothing is found.
    */
    int GameBoard::findOpenPosition(Vessel::VType vtype, VDirection vdir)
    {
        bool stopFlag = false;
        int result = GB_INVALID_POSITION;

        /* Get a random starting position for the vessel. */
        VBorder vrect;
        vrect.topX = randomNumber(0, lengthOfBoard_, false);
        vrect.topY = randomNumber(0, widthOfBoard_, false);

        size_t searchPos = coordinateIndex(vrect, true);
        size_t savePos = searchPos;

        while(!stopFlag)
        {
            /* 
            Verify search position doesn't exceed  bounds.
            This is always false on first pass.
            */
            if (searchPos >= board_.size())
                searchPos = 0;

            /* scan for open position if search position is occupied already */
            while(board_[searchPos].vesselId >= 0)
            {
                searchPos++;
                /* 
                Ensure we don't loop beyond starting point.
                searchPos may have been set to start at 0 above.
                */
                if(searchPos == savePos)
                {
                    stopFlag = true;
                    break;
                }
            }

            /* Process only stopFlag is not true */
            if (!stopFlag)
            {
                /* 
                At this stage we're at an open position.
                Determine whether vessel can fit here.
                Start by obtaining vessel boarder and map based 
                */
                std::tie(vrect.topX, vrect.topY) = coordinates(searchPos);
                std::vector<int> vmap = getPositionMap(vrect, vtype, vdir);

                /* Determine if the vessel area is valid */
                if(isValidBorder(vrect) == true && isAreaOccupied(vmap) == GB_INVALID_POSITION)
                {
                    result = searchPos;
                    stopFlag = true;
                }
                searchPos++;
            }
        }        
        return result;
    }

    /**
     Determine the list of board indexes that form the footprint of the given VesselType at the specified location.
     Set the (topX, topY) coordinates of vrect parameter before calling the function.
     If the (topX, topY) coordinates provided are invalid, the function will throw an Error.
     The function will adjust the vrect parameter with the full boarder for the specified vessel.
     Note that depending on the type of vessel specified in vtype and the (topX, topY) coordinates provided the 
     vessels footprint may exceed the boundary of the board. It's therefore recommended that the user validate the 
     vrect provided. If the vrect is valid, then the return vector contains valid gameboard positions.
     @param vrect
        The enclosing rectangular border of the vessel defined by the cordinates for the top left corner and lower right corner.
     @param vtype
        The type of vessel being placed on the game board.
     @param vdir
        The orientation to place the vessel on the game board.
     @return
        Return a vector of board indices constitute the footprint of the vessel on the board.
    */
    std::vector<int> GameBoard::getPositionMap(VBorder& vrect, Vessel::VType vtype, VDirection vdir) const
    {
        /* Validate the starting coordinates */
        if(!isValidTopXY(vrect))
        {
            Error argError("Invalid border top arguments supplied.", GB_ARG_ERROR, GB_ARG_FILTER, __FILE__, __LINE__);
            throw argError;
        }

        //obtain the vessels border
        border(vrect, vtype, vdir);

        std::vector<int> result;
        for(int idx = vrect.topX; idx < vrect.lowX; idx++)
        {
            for(int idy = vrect.topY; idy < vrect.lowY; idy++)
            {
                VBorder tmpBorder(idx, idy);
                result.push_back(coordinateIndex(tmpBorder));
            }
        }
        return result;
    }

    /**
     Determines whether the vessel's rectangular border area on the game board is occupied by another vessel.
     @param vmap
        A vector containing gameboard locations occupied by the vessel.
        This value can be obtained by calling getPositionMap();
     @return
        A positive number (or zero) representing the first index on the gameboard where another vessel was found.
        A negative number if the entire vessel area on the gameboard is unoccupied.
    */
    int GameBoard::isAreaOccupied(std::vector<int>& vmap) const
    {
        int result = GB_INVALID_POSITION;
        
        int mapSize = vmap.size();
        for(int idx = 0; idx < mapSize; idx++)
        {
            if(board_[vmap[idx]].vesselId >= 0)
            {
                result = vmap[idx];
                break;
            }
        }
        return result;
    }

    /**
     Determine if the border argument supplied fits within the confines of the game board.
     All coordinate values must be set for the vrect parameter before calling this function.
     @param vrect
        The enclosing rectangular border of the vessel defined by the cordinates for the top left corner and lower right corner.
     @return
        True if border coordinates are valid.
        False otherwise.
    */
    bool GameBoard::isValidBorder(VBorder& vrect) const
    {
        return (isValidTopXY(vrect) && isValidLowXY(vrect));
    }

    /**
     Determine if the upper corner coordinates provided for the argument border is within the board range.
     The (topX, topY) coordinate values must be set for the vrect parameter before calling this function.
     @param vrect
        The enclosing rectangular border of the vessel defined by the cordinates for the top left corner and lower right corner.
     @return
        True if upper corner coordinates are valid.
        False otherwise.
    */
    bool GameBoard::isValidTopXY(VBorder& vrect) const
    {
        if((0 <= vrect.topX && vrect.topX < lengthOfBoard_) && (0 <= vrect.topY && vrect.topY < widthOfBoard_))
        {
            return true;
        }
        return false;
    }

    /**
     Determine if the lower corner coordinates provided for the argument border is within the board range.
     The (lowX, lowY) coordinate values must be set for the vrect parameter before calling this function.
     @param vrect
        The enclosing rectangular border of the vessel defined by the cordinates for the top left corner and lower right corner.
     @return
        True if lower corner coordinates are valid.
        False otherwise.
    */
    bool GameBoard::isValidLowXY(VBorder& vrect) const
    {
        if((0 <= vrect.lowX && vrect.lowX < lengthOfBoard_) && (0 <= vrect.lowY && vrect.lowY < widthOfBoard_))
        {
            return true;
        }
        return false;
    }

    /**
     Convert the indexes for a two-dimensional array into its equivalent one-dimensional array index.
     By default the (topX, topY) coordinates are the ones processed, however setting the argument flag to false will
     use the (lowX, lowY) coordinates instead.
     This conversion succeeds only if the argument indices are valid for the array.
     Passing invalid arguments will result in an invalid_argument exception.
     @param vrect
        The enclosing rectangular border of the vessel defined by the cordinates for the top left corner and lower right corner.
     @param topFlag
        If topFlag is true then the (topX, topY) coordinate values are used to determine the index.
        If topFlag is false then the (lowX, lowY) coordinate values are used to determine the index.
     @return
        The corresponding one-dimensional array index.
    */
    int GameBoard::coordinateIndex(VBorder& vrect, bool topFlag) const
    {
        int result = -1;

        if((topFlag && !isValidTopXY(vrect)) || (!topFlag && !isValidLowXY(vrect)))
        {
            Error argError("Invalid coordinates supplied.", GB_ARG_ERROR, GB_ARG_FILTER, __FILE__, __LINE__);
            throw argError;
        }

        if(topFlag == true)
        {
            result = (vrect.topX * lengthOfBoard_) + vrect.topY;
        }
        else
        {
            result = (vrect.lowX * lengthOfBoard_) + vrect.lowY;
        }

        return result;
    }

    /**
     Convert a one dimensional array board index to a two dimensional logical index.
     @param index
        A valid game board index.
     @return
        An std::pair<x:y> representing the corresponding x, y coordinates into a logical 2-dimensional array
    */
    std::pair<int, int> GameBoard::coordinates(int index) const
    {
        if (index < 0)
        {
            Error argError("Invalid board index.", GB_ARG_ERROR, GB_ARG_FILTER, __FILE__, __LINE__);
            throw argError;
        }

        int row = index / lengthOfBoard_;
        int col = index % lengthOfBoard_;

        return std::make_pair(row, col);
    }

    /**
     Generate random numbers within the specified range. The number generated is the range [start, end]    
     If invalid values are passed for the start and end arguments, the function will throw a Cylink::Error.
     The random number generator should be seeded on first call by setting the third parameter to true.
     @param start
        The first number within the possible values specifying the range.
        This may be zero or a positive number.
     @param end
        The last possible number to generate within the range.
        This may not be less than or equal to the value of @param start
     @param seedFlag
        A flag to indicate whether or not the random number generator should be seeded.
        A value of true will cause the seeding of the generator before a random number is generated.
     @return
        A pseudo random number within the specified range.
    */
    int GameBoard::randomNumber(int start, int end, bool seedFlag)
    {
        if(start < 0 || end < 0 || start >= end)
        {
            Error argError("Invalid number range supplied.", GB_ARG_ERROR, GB_ARG_FILTER, __FILE__, __LINE__);
            throw argError;
        }

        if(seedFlag == true)
        {
            srand(static_cast<unsigned int>(time(NULL)));
        }
        return start + (rand() % end);
    }

    /**
     The border of a vesel is the smallest enclosing rectangle that can contain the vessel based on the vessel length and width.
     The enclosing rectangle is specified by the coordinates for the top left corner and lower right corner.
     This function calculates the lower right corner of the enclosing rectangle based on the coordinates of the top left corner.
     The result is then written back into the border argument supplied.
     @param vrect
        This parameter is both an input and output parameter.
        The border must have the values for (topX, topY) specified and the function will calculate and overwrite (lowX, lowY)
     @param vtype
        The type of vessel.
     @param vdir
        The direction of the vessel ie. vertical or horizontal.
    */
    void GameBoard::border(VBorder& vrect, Vessel::VType vtype, VDirection vdir)
    {
        int vesselLength = 0;
        int vesselWidth = 0;
        vrect.lowX = -1;
        vrect.lowY = -1;

        std::tie(vesselLength, vesselWidth) = Vessel::vesselDimensions(vtype);
        switch(vdir)
        {
        case VDirection::HORIZONTAL:
            vrect.lowX = vrect.topX + vesselWidth;
            vrect.lowY = vrect.topY + vesselLength;
            break;

        case VDirection::VERTICAL:
            vrect.lowX = vrect.topX + vesselLength;
            vrect.lowY = vrect.topY + vesselWidth;
            break;
        }
    }

    /**
     Print the game board grid to the specified output stream
     @param os
        A standard output stream to print the game board to.
     @param vs
        The gameboard to print.
     @return
        A reference to the output stream.
    */
    std::ostream& operator<<(std::ostream& os, const GameBoard& gb)
    {
        /* print colum*/
        os<<"\n"<<std::setw(BOARD_SQUARE)<<" ";
        for(int x=0; x<gb.lengthOfBoard_; x++)
        {
            os<<std::setw(BOARD_SQUARE)<<StringUtils::centered(std::to_string(x+1));
        }
        os<<"\n";

        for(int x=0; x<gb.lengthOfBoard_; x++)
        {
            os<<std::setw(BOARD_SQUARE)<<StringUtils::centered(std::to_string(x+1));
            for(int y=0; y<gb.widthOfBoard_; y++)
            {
                GameBoard::VBorder vrect(x, y);
                int coordinateIndex = gb.coordinateIndex(vrect);
                int vesselId = gb.board_[coordinateIndex].vesselId;
                Vessel vs = gb.vessels_[vesselId];
                os<<std::setw(BOARD_SQUARE)<<StringUtils::centered(Vessel::formatVessel(vs.getType(), false));
            }
        }
        return os;
    }
}