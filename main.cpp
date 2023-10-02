//CATCH_CONFIG_RUNNER tells the catch library that this 
//project will now explicitly call for the tests to be run. 
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

#include <iostream>
#include <string>
#include "player.h"

#define BOARDX 10
#define BOARDY 10

//A macro used in main to determine if you want to run
//the tests or not. If you don't want to run your tests,
//change true to false in the line below.
#define PERFORM_TESTS true
 
/*
* runCatchTests will cause Catch to go ahead and
* run your tests (that are contained in the tests.cpp file.
* to do that, it needs access to the command line
* args - argc and argv. It returns an integer that
* ultimately gets passed back up to the operating system.
* See the if statement at the top of main for
* a better overview.
*/
int runCatchTests(int argc, char* const argv[])
{
    //This line of code causes the Catch library to 
    //run the tests in the project. 
    return Catch::Session().run(argc, argv);
}
 
int main( int argc, char* const argv[] )
{
    //If the PERFORM_TESTS macro is defined to be true,
    //runCatchTests will be called and immediately
    //return causing the program to terminate. Change TEST
    //to false in the macro def at the top of this file
    //to skip tests and run the rest of your code.
    if (PERFORM_TESTS)
    {
        return runCatchTests(argc, argv);
    }

    //start working on other parts of your project here.

    using Cylink::Player;
    using Cylink::VesselType;

    /* Create players */
    Player player1(BOARDX, BOARDY);
    Player player2(BOARDX, BOARDY);

    /* Add vessels to each player1's board */
    player1.addVessel(VesselType::CARRIER, 1);
    player1.addVessel(VesselType::CRUISER, 3);
    player1.addVessel(VesselType::DESTROYER, 3);
    player1.addVessel(VesselType::FRIGATE, 2);
    player1.addVessel(VesselType::SUBMARINE, 2);

    /* Add vessels to each player2's board */
    player2.addVessel(VesselType::CARRIER, 1);
    player2.addVessel(VesselType::CRUISER, 3);
    player2.addVessel(VesselType::DESTROYER, 3);
    player2.addVessel(VesselType::FRIGATE, 2);
    player2.addVessel(VesselType::SUBMARINE, 2);





    return 0;
}
