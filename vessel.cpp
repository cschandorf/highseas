#include "vessel.h"

using std::pair;

namespace Cylink
{
    /**
     Create a Vessel of the designated type and set the damage according to the type of vessel.
     The default type if not specified is a GUNBOAT.
     @param vtype
        The type of vessel to create. By default this is a VesselType::GUNBOAT.
    */
    Vessel::Vessel(VesselType vtype)
        : vesselType_(vtype), damageLevel_(0)
    {
        damageLevel_ = getDamageLevel(true);
    }

    /**
     Create a new Vessel based on an existing Vessel.
     @param other
        The existing Vessel object that's used to initialize the current object.
    */
    Vessel::Vessel(const Vessel& other)
        : vesselType_(other.vesselType_), damageLevel_(other.damageLevel_)
    {
    }

    /**
     Release any resources held by the vessel object.
    */
    Vessel::~Vessel()
    {
    }

    /**
     Make an existing vessel same as the argument vessel.
     @param other
        The vessel whose values are used to set the current object.
     @return
        A reference to the current object.
    */
    Vessel& Vessel::operator= (const Vessel& other)
    {
        if(this != &other)
        {
            vesselType_ = other.vesselType_;
            damageLevel_ = other.damageLevel_;
        }
        return *this;
    }

    /**
     Get the length of the current vessel.
     @return
        The length of the current vessel.
    */
    int Vessel::getLength() const
    {
        return getDimensions().first;
    }

    /**
     Get the width of the current vessel.
     @return
        The width of the current vessel.
    */
    int Vessel::getWidth() const
    {
        return getDimensions().second;
    }

    /**
     Get the dimensions of the current vessel.
     @return
        The dimensions of the current vessel as the pair<length, width>
    */
    std::pair<int, int> Vessel::getDimensions() const
    {
        return Vessel::getVesselDimensions(vesselType_);
    }

    /**
     Obtain the type of the current vessel.
     @return 
        The VesselType of the current vessel.
    */
    VesselType Vessel::getType() const
    {
        return vesselType_;
    }

    /**
     Function is called when the ship is hit by an enemy. 
     The function reduces the damageLevel of the vessel by the value of the shot and returns the remaining damage level.
     @param shot
        Represents the strength of the shot hitting the vessel. The amount of damage taken is determined by the shot.
     @return 
        The remaining damage value, adjusted so it never goes negative. A value of 0 implies the vessel is sunk.
    */
    int Vessel::takeHit(int shot)
    {
        damageLevel_ -= shot;
        if(damageLevel_ < 0)
            damageLevel_ = 0;

        return damageLevel_;
    }

    /**
     Obtain the amount of damage the vessel can take before sinking. 
     The damage level is based on the type of vessel and set when the vessel is initially constructed.
     This function can either return the initial damage level or the current damage level.
     The current damage level may be lower than the initial level if the vessel has successfully been hit in the past.
     @param initFlag
        Set this to true to obtain the initial damage level. The default flag value is false.
     @return
        The damage level of the current vessel.
    */
    int Vessel::getDamageLevel(bool initFlag)
    {
        return (initFlag) ? getLength() * getWidth() : damageLevel_;
    }

    /**
     Determine the arbitrarily set dimensions for various vessel types.
     @param vtype
        A VesselType value for which to obtain dimensions.
    @return
        An std::pair value represented as pair<length:width>.
    */
    std::pair<int, int> Vessel::getVesselDimensions(VesselType vtype)
    {
        std::pair<int, int> result(-1, -1);

        switch(vtype)
        {
        case VesselType::GUNBOAT:
            return std::make_pair(1,1);

        case VesselType::CRUISER:
            return std::make_pair(2, 1);

        case VesselType::FRIGATE:
            return std::make_pair(3, 1);

        case VesselType::DESTROYER:
            return std::make_pair(4, 2);

        case VesselType::SUBMARINE:
            return std::make_pair(6, 1);

        case VesselType::CARRIER:
            return std::make_pair(7, 2);
        }

        return result;
    }

    /**
     Create a short 2-letter string or long descriptive string representing the specified VesselType.
     @param vt
        The type of the vessel for which a descriptive string is provided.
     @return 
        A descriptive string for the VesselType.
    */
    std::string Vessel::formatVessel(VesselType vt, bool printLong)
    {
        switch(vt)
        {
        case VesselType::CARRIER:
            return (printLong) ? "Carrier" : "CA";
        case VesselType::CRUISER:
            return (printLong) ? "Cruiser" : "CR";
        case VesselType::DESTROYER:
            return (printLong) ? "Destroyer" : "DE";
        case VesselType::FRIGATE:
            return (printLong) ? "Frigate" : "FR";
        case VesselType::GUNBOAT:
            return (printLong) ? "Gunboat" : "GB";
        case VesselType::SUBMARINE:
            return (printLong) ? "Submarine" : "SM";
        }
        return "";
    }

    /**
     Write a short two letter code indicating the VesselType to the supplied output stream.
     @param os
        A standard output stream to which 2-letter code for the vessel is written.
     @param vs
        The vessel whose 2-letter code is output.
     @return
        A reference to the output stream.
    */
    std::ostream& operator<<(std::ostream& os, const Vessel& vs)
    {
        os<<Vessel::formatVessel(vs.vesselType_, false);
        return os;
    }

    /**
     Write the type of vessel to the supplied output stream.
     @param os
        A standard output stream to write to.
     @param vt
        The vessel type.
     @return
        A reference to the output stream.
    */
    std::ostream& operator<<(std::ostream& os, const VesselType& vt)
    {
        os<<Vessel::formatVessel(vt, true);
        return os;
    }
}
