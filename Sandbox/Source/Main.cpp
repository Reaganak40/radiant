#include "Sandbox.h"
#include "Scenes/World.h"

using namespace rdt;

int main(void) {

	Application app;
    app.Start("Sandbox");
    
    /* Set the initial Scene, */
    app.AddScene("world", new World);
    app.SetScene("world");

    /* Run the game loop for the rest of the application. */
    app.RunGameLoop();

	return 0;
}