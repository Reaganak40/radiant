#include "Pong.h"
#include "GameObjects/Padel.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Wall.h"

#include "UI/Scoreboard.h"

using namespace Radiant;

int main(void)
{
    Application pongApp;
    pongApp.Start("Pong");

    Renderer::SetBackgroundColor(BLACK);

    Padel* player1;
    pongApp.AddGameObject(player1 = new Padel(30 + (PADEL_WIDTH / 2), pongApp.WindowHeight() / 2));
    player1->SetRightControl({});
    player1->SetLeftControl({});

    Padel* player2;
    pongApp.AddGameObject(player2 = new Padel(pongApp.WindowWidth() - (PADEL_WIDTH / 2) - 30, pongApp.WindowHeight() / 2));
    player2->SetUpControl(std::vector<InputState>{UP_KEY_DOWN, UP_KEY_PRESS});
    player2->SetDownControl(std::vector<InputState>{DOWN_KEY_DOWN, DOWN_KEY_PRESS});
    player2->SetRightControl({});
    player2->SetLeftControl({});

    Ball* ball;
    pongApp.AddGameObject(ball = new Ball(pongApp.WindowWidth() * 0.85, pongApp.WindowHeight() / 2));

    Wall* top;
    pongApp.AddGameObject(top = new Wall(pongApp.WindowWidth() / 2, pongApp.WindowHeight() - (WALL_HEIGHT/2)));
    top->SetWallVisibility(true);

    Wall* bottom;
    pongApp.AddGameObject(bottom = new Wall(pongApp.WindowWidth() / 2, WALL_HEIGHT / 2));
    bottom->SetWallVisibility(true);

    Renderer::AttachGui(new Scoreboard(*ball));
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