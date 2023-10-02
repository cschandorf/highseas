#include <catch2/catch_test_macros.hpp>
#include "vessel.h"

TEST_CASE ("Testing Vessels", "[Vessel]")
{
    Cylink::Vessel default_vessel;                              //Default constructed vessel.
    Cylink::Vessel con_vessel(Cylink::VesselType::CARRIER);     //Constructed vessel
    Cylink::Vessel copy_con_vessel(default_vessel);             //Copy constructed vessel

    SECTION("Vessel Initialization")
    {
        std::cout<<"Testing vessel initialization"<<std::endl;

        /* Constructed vessels should match their expected types */
        REQUIRE(default_vessel.getType() == Cylink::VesselType::GUNBOAT);
        REQUIRE(con_vessel.getType() == Cylink::VesselType::CARRIER);
        REQUIRE(copy_con_vessel.getType() == Cylink::VesselType::GUNBOAT);

        Cylink::Vessel v1(Cylink::VesselType::FRIGATE);
        REQUIRE(v1.getType() == Cylink::VesselType::FRIGATE);

        /* Verify assignment operator changes vessel type */
        copy_con_vessel = v1;
        REQUIRE(copy_con_vessel.getType() == Cylink::VesselType::FRIGATE);
    }

    SECTION("Testing Print Format")
    {
        /* Verify short formats -- This is done mostly so if it changes in code we catch it */
        std::cout<<"Testing vessel's short print formats"<<std::endl;
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::CARRIER) == "CA");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::CRUISER) == "CR");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::DESTROYER) == "DE");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::FRIGATE) == "FR");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::GUNBOAT) == "GB");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::SUBMARINE) == "SM");

        /* Verify long formats -- This is done mostly so if it changes in code we catch it */
        std::cout<<"Testing vessel's long print formats"<<std::endl;
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::CARRIER, true) == "Carrier");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::CRUISER, true) == "Cruiser");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::DESTROYER, true) == "Destroyer");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::FRIGATE, true) == "Frigate");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::GUNBOAT, true) == "Gunboat");
        REQUIRE(Cylink::Vessel::formatVessel(Cylink::VesselType::SUBMARINE, true) == "Submarine");
    }

    SECTION("Verifying Vessel Dimensions")
    {
        /* Verify that known vessels have expected dimensions */
        std::cout<<"Verifying that specific vessels have expected dimensions\n";

        std::cout<<"Verify "<<default_vessel<<" width = "<<default_vessel.getWidth() <<" and height = "<<default_vessel.getLength()<<"\n";
        CHECK(default_vessel.getLength() == 1);
        CHECK(default_vessel.getWidth() == 1);

        std::cout<<"Verify "<<con_vessel<<" width = "<<con_vessel.getWidth() <<" and height = "<<con_vessel.getLength()<<"\n";
        CHECK(con_vessel.getLength() == 7);
        CHECK(con_vessel.getWidth() == 2);

        std::cout<<"Verify "<<copy_con_vessel<<" width = "<<copy_con_vessel.getWidth() <<" and height = "<<copy_con_vessel.getLength()<<"\n";
        CHECK(copy_con_vessel.getLength() == 1);
        CHECK(copy_con_vessel.getWidth() == 1);
    }
}