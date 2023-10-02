#include <tuple>
#include <cstdlib>

#include "gameboard.h"
#include "error.h"
#include "utils.h"

#define BOARD_SQUARE 5
#define ARG_FILTER "INVALID_ARG"
#define ARG_ERROR "CY0000"

namespace Cylink
{
    /**
     Create a game board with the given dimensions. The board doesn't have to be symetric.
     Negative of zero argument values for length or height will result in an exception.
     @param length
        The length of the board. Defaults to BOARD_SIZE if unspecified. Value must be greater than zero.
     @param height
        The height of the board. Defaults to BOARD_SIZE if unspecified. Value must be greater than zero.
    */
    GameBoard::GameBoard(int length, int height)
        : lengthOfBoard_(length), heightOfBoard_(height), board_(), warships_()
    {
        if(lengthOfBoard_ <= 0 || heightOfBoard_ <= 0)
        {
            Error argError("Board length and height must be greater than zero.", ARG_ERROR, ARG_FILTER, __FILE__, __LINE__);
            throw argError;
        }
    }

    /**
     The game board is copy constructible.
     @param other
        An existing GameBoard object used to copy initialize the new object.
    */
    GameBoard::GameBoard(const GameBoard& other)
        : lengthOfBoard_(other.lengthOfBoard_), heightOfBoard_(other.heightOfBoard_), board_(other.board_), warships_(other.warships_)
    {
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
            heightOfBoard_ = other.heightOfBoard_;
            /* Copy the contents between vectors below */
            board_ = other.board_;
            warships_ = other.warships_;
        }
        return *this;
    }

    /**
     Put a vessel on the game board if it's determined to meet the required criteria.
     Vessels may not overlap on the same game board, though another player may have a vessel in the same position on their board.
     The vessels must fit at the specified location on the game board given it's start position, orientation and size.
     @param xpos
        The index selecting the row of a two-dimensional array.
     @param ypos
        The index selecting the column of a two-dimensional array.
     @param vtype
        The type of vessel being placed on the game board.
     @param direction
        The orientation to place the vessel on the game board.
     @return
        Return true if the indicated vessel can be fit on the gameboard at the suggested location.        
    */
    bool GameBoard::emplaceVessel(int xpos, int ypos, VesselType vtype, Orientation direction)
    {
        bool result = false;
        //Determine if vessel will fit on game board.
        if(fitVessel(xpos, ypos, vtype, direction))
        {
            //Get indexes occupied by the vessel
            std::vector<int> positionMap = getPositionMap(xpos, ypos, vtype, direction); 
            int mSize = positionMap.size();
            if(mSize > 0)
            {
                //Add vessel to warship list
                warships_.push_back(Vessel(vtype));
                int shipIndex = warships_.size()-1;
                //Update board with vessel position
                for(int idx=0; idx<mSize; idx++)
                {
                    board_[positionMap[idx]].shipLocation = shipIndex;
                }
                //set the result
                result = true;
            }
        }
        return result;
    }

    std::pair<int, int> GameBoard::getValidPosition(VesselType vtype, Orientation direction)
    {
        /* Generate random coordinates */
        int length = 0;
        int width = 0;
        int vesselLength = 0;
        int vesselWidth = 0;

        //obtain dimensions of specified vessel
        std::tie(vesselLength, vesselWidth) = Vessel::getVesselDimensions(vtype);

        switch(direction)
        {
        case Orientation::HORIZONTAL:
            length = randomNumber(0, lengthOfBoard_ - vesselLength);
            width = randomNumber(0, heightOfBoard_);
            break;

        case Orientation::VERTICAL:
            length = randomNumber(0, lengthOfBoard_);
            width = randomNumber(0, heightOfBoard_ - vesselWidth);
            break;
        }

        /* Determine whether */
    }

    /**
     Determine the list of board indexes that form the footprint of the given VesselType at the specified location.
     @param xpos
        The index selecting the row of a two-dimensional array.
     @param ypos
        The index selecting the column of a two-dimensional array.
     @param vtype
        The type of vessel being placed on the game board.
     @param direction
        The orientation to place the vessel on the game board.
     @return
        Return a vector of board indices constitute the footprint of the vessel on the board.
    */
    std::vector<int> GameBoard::getPositionMap(int xpos, int ypos, VesselType vtype, Orientation direction)
    {
        int vesselLength = 0;
        int vesselWidth = 0;
        std::vector<int> result;

        //obtain dimensions of specified vessel
        std::tie(vesselLength, vesselWidth) = Vessel::getVesselDimensions(vtype);

        //adjust the lengths
        switch(direction)
        {
        case Orientation::HORIZONTAL:
            vesselLength += xpos;
            vesselWidth += ypos;
            break;

        case Orientation::VERTICAL:
            vesselLength += ypos;
            vesselWidth += xpos;
            break;
        }

        for(int x=xpos; x<vesselLength; x++)
        {
            for(int y=ypos; y<vesselWidth; y++)
            {
                result.push_back(GameBoard::getCoordinateIndex(x, y));
            }
        }
        return result;
    }

    /**
     Determine whether or not a specific vessel type can fit on the game board given the suggested
     start coordinates, the vessel dimensions and the orientation to place it. 
     @param xpos
        The index selecting the row of a two-dimensional array.
     @param ypos
        The index selecting the column of a two-dimensional array.
     @param vtype
        The type of vessel being placed on the game board.
     @param direction
        The orientation to place the vessel on the game board.
     @return
        Return true if the indicated vessel can be fit on the gameboard at the suggested location.        
    */
    bool GameBoard::fitVessel(int xpos, int ypos, VesselType vtype, Orientation direction)
    {
        bool result = false;

        int bottomX = 0;
        int bottomY = 0;
        int vLength = 0;
        int vWidth = 0;

        //Obtain dimensions of specified vessel
        std::tie(vLength, vWidth) = Vessel::getVesselDimensions(vtype);

        //Determine bottom coordinates
        switch(direction)
        {
        case Orientation::HORIZONTAL:
            bottomX = xpos + vLength;
            bottomY = ypos + vWidth;
            break;

        case Orientation::VERTICAL:
            bottomX = ypos + vLength;
            bottomY = xpos + vWidth;
            break;
        }

        //Does this intersect with the board
        if((0 <= xpos && xpos < lengthOfBoard_) && (0 <= ypos && ypos < heightOfBoard_))
        {
            if((0 <= bottomX && bottomX < lengthOfBoard_) && (0 <= bottomY && bottomY < heightOfBoard_))
                result = true;
        }

        return result;
    }

    /**
     Convert the indexes for a two-dimensional array into its equivalent one-dimensional array index.
     This conversion succeeds only if the argument indices are valid for the array.
     Passing invalid arguments will result in an invalid_argument exception.
     @param xpos
        The index selecting the row of a two-dimensional array.
     @param ypos
        The index selecting the column of a two-dimensional array.
     @return
        The corresponding one-dimensional array index.
    */
    int GameBoard::getCoordinateIndex(int xpos, int ypos) const
    {
        if(xpos < 0 || xpos >= lengthOfBoard_ || ypos < 0 || ypos >= heightOfBoard_)
        {
            Error argError("Invalid coordinates supplied.", ARG_ERROR, ARG_FILTER, __FILE__, __LINE__);
            throw argError;
        }
        return (xpos * lengthOfBoard_) + ypos;
    }

    /**
     Generate random numbers within the specified range. The number generated is the range [start, end]
     If the value of start is unspecified it defaults to zero.
     If invalid values are passed for the start and end arguments, the function will throw a Cylink::Error.
     @param start
        The first number within the possible values specifying the range.
        This may be zero or a positive number.
     @param end
        The last possible number to generate within the range.
        This may not be less than or equal to the value of @param start
     @return
        A pseudo random number within the specified range.
    */
    int GameBoard::randomNumber(int start, int end)
    {
        if(start < 0 || end < 0 || start >= end)
        {
            Error argError("Invalid number range supplied.", ARG_ERROR, ARG_FILTER, __FILE__, __LINE__);
            throw argError;
        }
        srand(static_cast<unsigned int>(time(NULL)));
        return start + (rand() % end);
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
            for(int y=0; y<gb.heightOfBoard_; y++)
            {
                int coordinateIndex = gb.getCoordinateIndex(x, y);
                int shipLocation = gb.board_[coordinateIndex].shipLocation;
                Vessel vs = gb.warships_[shipLocation];
                os<<std::setw(BOARD_SQUARE)<<StringUtils::centered(Vessel::formatVessel(vs.getType()));
            }
        }
        return os;
    }
}