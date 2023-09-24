#include "Sandbox.h"
#include "Scenes/World.h"

using namespace rdt;

int main(void) {

	Application app;
	app.Start("Sandbox");

    app.AddScene("world", new World);
    app.SetScene("world");

    /* Loop until the user closes the window */
    while (app.IsRunning())
    {
        /* Clears window and prepares for next game loop.*/
        app.BeginFrame();

        /* Process input from user and update game objects. */
        app.ProcessInput();

        /* Update the physical world. Detect and resolve collisions. */
        app.UpdateWorld();

        /* Final update of game objects before render. */
        app.FinalUpdate();

        /* Render the current frame. */
        app.Render();

        /* End the Frame, poll inputs, swap buffers. */
        app.EndFrame();
    }

	return 0;
}