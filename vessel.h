#ifndef VESSEL_H
#define VESSEL_H

#include <iostream>
#include <utility>

/**
 * @namespace Cylink
 * General project namespace
 */
namespace Cylink
{
    /**
     Class represents the various ships used in the HighSeas game. 
    */
    class Vessel
    { 
    public:
        /**
         @enum VType
         Specify the types of possible vessels in the game.
        */
        enum class VType
        {
            GUNBOAT,
            CRUISER,
            FRIGATE,
            DESTROYER,
            SUBMARINE,
            CARRIER
        };

    public:
        Vessel(VType vtype = VType::GUNBOAT);
        Vessel(const Vessel& other);
        ~Vessel();
        Vessel& operator =(const Vessel& other);

        int takeHit(int shot);

        //Constant functions
        int getLength() const;
        int getWidth() const;
        std::pair<int, int> getDimensions() const;

        int getDamageLevel(bool initFlag = false);
        VType getType() const;

        //Static functions
        static std::pair<int, int> vesselDimensions(VType vtype);
        static std::string formatVessel(VType vt, bool printLong = false);

        //friend declarations
        friend std::ostream& operator<<(std::ostream& os, const Vessel& vs);
        friend std::ostream& operator<<(std::ostream& os, const VType& vt);
        
    private:
        VType vesselType_;      /**< Designates the type of vessel */
        int damageLevel_;       /**< Tracks amount of damage vessel has taken. */
    };
}

#endif