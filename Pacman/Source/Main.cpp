#include "Game.h"

#include "Scenes/Level.h"

using namespace rdt;

int main(void)
{
    Application pacmanApp;
    pacmanApp.Start("Pacman", SCREEN_WIDTH, SCREEN_HEIGHT, false);

    pacmanApp.AddScene("Level", new Level);
    pacmanApp.SetScene("Level");

    /* Loop until the user closes the window */
    while (pacmanApp.IsRunning())
    {
        /* Clears window and prepares for next game loop.*/
        pacmanApp.BeginFrame();

        /* Get start of frame messages. */
        pacmanApp.PollMessages1();

        /* Process input from user and update game objects. */
        pacmanApp.ProcessInput();

        /* Update the physical world. Detect and resolve collisions. */
        pacmanApp.UpdateWorld();

        /* Get late update messages. */
        pacmanApp.PollMessages2();

        /* Final update of game objects before render. */
        pacmanApp.FinalUpdate();

        /* Render the current frame. */
        pacmanApp.Render();

        /* End the Frame, poll inputs, swap buffers. */
        pacmanApp.EndFrame();
    }


    return 0;
}