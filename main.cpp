//CATCH_CONFIG_RUNNER tells the catch library that this 
//project will now explicitly call for the tests to be run. 
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

#include <iostream>
#include <string>
#include "player.h"

#define BOARDX 10
#define BOARDY 10
#define PLAYER1_ATTACK 1
#define PLAYER2_ATTACK 2

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

    /* ======== start working on other parts of your project here. ======== */

    //Create player resources.
    Cylink::Player player1(BOARDX, BOARDY);
    Cylink::Player player2(BOARDX, BOARDY);

    //Create vector of game vessels
    std::vector<Cylink::Vessel::VType> vessels;
    vessels.insert(vessels.end(), 1, Cylink::Vessel::VType::CARRIER);
    vessels.insert(vessels.end(), 3, Cylink::Vessel::VType::CRUISER);
    vessels.insert(vessels.end(), 3, Cylink::Vessel::VType::DESTROYER);
    vessels.insert(vessels.end(), 2, Cylink::Vessel::VType::FRIGATE);
    vessels.insert(vessels.end(), 2, Cylink::Vessel::VType::SUBMARINE);

    //register vessels with players
    int shipCount1 = player1.setupBoard(vessels);
    int shipCount2 = player2.setupBoard(vessels);

    //Verify that both players have same number of ships
    if(shipCount1 == shipCount2)
    {
        /* Game starts with a random player then simulation continues */
        int xCord, yCord;
        int playerSelector = Cylink::GameBoard::randomNumber(1, 2, true);
        bool gameOver = false;
        do
        {
            switch(playerSelector)
            {
            /* Player 1 launches attack */
            case PLAYER1_ATTACK:
                //Get a fire position suggestion.
                std::tie(xCord, yCord) = player1.suggestFirePosition();

                //launch the attack.
                player1.launchAttack(player2, xCord, yCord);

                //determine if game is over because player2 is dead.
                if(player2.hasVessels() == false)
                {
                    gameOver = true;
                }

                //toggle the player selector to the opponent.
                playerSelector = PLAYER2_ATTACK;
                break;

            /* Player 2 launches attack */
            case PLAYER2_ATTACK:
                //Get a fire position suggestion.
                std::tie(xCord, yCord) = player2.suggestFirePosition();

                //launch the attack.
                player2.launchAttack(player1, xCord, yCord);

                //determine if game is over because player2 is dead.
                if(player1.hasVessels() == false)
                {
                    gameOver = true;
                }

                //toggle the player selector to the opponent.
                playerSelector = PLAYER1_ATTACK;
                break;
            };

        } while (gameOver == false);
    }

    return 0;
}
