#include "Game.h"
#include "Scenes/Level.h"

using namespace rdt;

int main(void)
{
    Application pacmanApp;
    pacmanApp.Start("Pacman", SCREEN_WIDTH, SCREEN_HEIGHT, false);

    pacmanApp.AddScene("Level", new Level);
    pacmanApp.SetScene("Level");

    /* Run the game loop for the rest of the application. */
    pacmanApp.RunGameLoop();
    
    return 0;
}