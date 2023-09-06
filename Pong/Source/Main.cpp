#include "Pong.h"
#include "GameObjects/Padel.h"

using namespace Radiant;

int main(void)
{
    Application pongApp;
    pongApp.Start("Pong");

    const UniqueID RedPlayer = pongApp.AddGameObject(new Padel(10.0 + (PADEL_WIDTH / 2), 300.0));

    Renderer::AttachGui(new DiagnosticsGUI);

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