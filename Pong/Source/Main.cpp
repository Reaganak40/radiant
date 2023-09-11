#include "Pong.h"
#include "Scenes/MainMenu.h"
#include "Scenes/Arena.h"

using namespace rdt;

int main(void)
{
    Application pongApp;
    pongApp.Start("Pong");

    // Define scenes
    pongApp.AddScene("MainMenu", new MainMenu);
    pongApp.AddScene("Arena", new Arena);

    // Set the initial scene.
    pongApp.SetScene("MainMenu");

    /* Loop until the user closes the window */
    while (pongApp.IsRunning())
    {
        /* Clears window and prepares for next game loop.*/
        pongApp.BeginFrame();
        
        /* Process input from user and update game objects. */
        pongApp.ProcessInput();

        /* Update the physical world. Detect and resolve collisions. */
        pongApp.UpdateWorld();

        /* Final update of game objects before render. */
        pongApp.FinalUpdate();

        /* Render the current frame. */
        pongApp.Render();

        /* End the Frame, poll inputs, swap buffers. */
        pongApp.EndFrame();
    }


    return 0;
}