// rabbits.cpp

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include"globals.h"
#include"Rabbit.h"
#include"Player.h"
#include"Game.h"
#include"Arena.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);

      // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANYTHING BETWEEN HERE AND THE END OF THE FILE!!!
// YOU MAY MOVE TO ANOTHER FILE ALL THE CODE FROM HERE TO THE END OF FILE, BUT
// BE SURE TO MOVE *ALL* THE CODE; DON'T MODIFY OR REMOVE ANY #IFDEF, ETC.
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++/g31/g32 UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)