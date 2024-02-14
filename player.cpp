#include "player.h"

namespace Cylink
{
   /**
    Create a player for battle ship and assign them a board of the size specified by the parameters.
   If no parameters are specified a 10 x 10 board is assumed and created.
   @param boardLength
   The length of board.
   @param boardHeight
   The height of the board.
   */
   Player::Player(int boardLength, int boardHeight)
   : board_(boardLength, boardHeight)
   {
   }

   /**
    The player is copy constructible.
   @param other
   An existing Player object used to copy initialize the new object.
   */
   Player::Player(const Player& other)
   : board_(other.board_)
   {
   }

   /**
    Release any allocated resources
   */
   Player::~Player()
   {
   }

   /**
    The player is copy assignable.
   @param other
   An existing Player object used to assign the current objects members.
   @return 
   Return a reference to the current object.
   */
   Player& Player::operator = (const Player& other)
   {
      if(this != &other)
      {
         board_ = other.board_;
      }
      return *this;
   }

   /**
    Sets up the player's game board by adding the supplied vessels to the board.
    The vessels are added to random positions on the player's board.
    @param vessels
      A vector of vessel types to add to the player's game board.
    @return
      The count of vessels successfully added to the player's game board.
   */
   int Player::setupBoard(const std::vector<Vessel::VType>& vessels)
   {
      int result = 0;
      for(auto ship : vessels)
      {
         if(board_.addVessel(ship))
         {
            result++;
         }
      }
      return result;
   }

   std::pair<int, int> Player::suggestFirePosition()
   {
   }

   void Player::launchAttack(Player& other, int xCord, int yCord)
   {
      GameBoard::VBorder vrect(xCord, yCord);
      //update opponents game board with attack information.
      GameBoard::StrikeResult sResult = other.board_.logReceivedAttack(vrect);
      //log the launch information 
      board_.logLaunchedAttack(vrect, sResult);
   }

   int Player::receiveAttack(int xpos, int ypos)
   {
   }

   bool Player::hasVessels()
   {
   }

}