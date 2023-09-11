#include "Game.h"

#include "Scenes/Level.h"

using namespace rdt;

int main(void)
{
    Application pacmanApp;
    pacmanApp.Start("Pacman", 960, 720, false);

    pacmanApp.AddScene("Level", new Level);
    pacmanApp.SetScene("Level");

    /* Loop until the user closes the window */
    while (pacmanApp.IsRunning())
    {
        /* Clears window and prepares for next game loop.*/
        pacmanApp.BeginFrame();

        /* Process input from user and update game objects. */
        pacmanApp.ProcessInput();

        /* Update the physical world. Detect and resolve collisions. */
        pacmanApp.UpdateWorld();

        /* Final update of game objects before render. */
        pacmanApp.FinalUpdate();

        /* Render the current frame. */
        pacmanApp.Render();

        /* End the Frame, poll inputs, swap buffers. */
        pacmanApp.EndFrame();
    }


    return 0;
}