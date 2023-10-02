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
      @enum VesselType
      Specify the types of possible vessels in the game.
    */
    enum class VesselType
    {
        GUNBOAT,
        CRUISER,
        FRIGATE,
        DESTROYER,
        SUBMARINE,
        CARRIER
    };

    /**
     Class represents the various ships used in the BattleShip game. 
     The VesselType determines how large the ship is and how much damage it can take before sinking.
     
     TODO: 
     Make various vessels have different attack strengths thereby inflicting more damage in a single shot?
     Perhaps as it takes on damange the strength of attack is also weakened?
    */
    class Vessel
    {       
    public:
        Vessel(VesselType vtype = VesselType::GUNBOAT);
        Vessel(const Vessel& other);
        ~Vessel();
        Vessel& operator =(const Vessel& other);

        int takeHit(int shot);

        //Constant functions
        int getLength() const;
        int getWidth() const;
        int getDamageLevel(bool initFlag = false);
        VesselType getType() const;
        std::pair<int, int> getDimensions() const;

        //Static functions
        static std::pair<int, int> getVesselDimensions(VesselType vtype);
        static std::string formatVessel(VesselType vt, bool printLong = false);

        //friend declarations
        friend std::ostream& operator<<(std::ostream& os, const Vessel& vs);
        friend std::ostream& operator<<(std::ostream& os, const VesselType& vt);
        
    private:
        VesselType vesselType_;     /**< Designates the type of vessel */
        int damageLevel_;           /**< Tracks amount of damage vessel has taken. */
    };
}

#endif