#include "Pong.h"
#include "GameObjects/Ball.h"

using namespace Radiant;

int main(void)
{
    Application pongApp;
    pongApp.Start("Pong");

    Ball red_ball(300.0, 300.0);

    Ball blue_ball(500.0, 500.0);
    blue_ball.SetSpriteColor(Color(ColorType::Blue));
    blue_ball.SetLeftControl(std::vector<InputState>{J_KEY_PRESS, J_KEY_DOWN});
    blue_ball.SetRightControl(std::vector<InputState>{L_KEY_PRESS, L_KEY_DOWN});
    blue_ball.SetUpControl(std::vector<InputState>{I_KEY_PRESS, I_KEY_DOWN});
    blue_ball.SetDownControl(std::vector<InputState>{K_KEY_PRESS, K_KEY_DOWN});

    Renderer::AttachGui(new DiagnosticsGUI);

    /* Loop until the user closes the window */
    while (pongApp.IsRunning())
    {
        /* Update App and run update procedures. */
        pongApp.Update();
        red_ball.OnUpdate(pongApp.GetDeltaTime());
        blue_ball.OnUpdate(pongApp.GetDeltaTime());

        if (Collision::CheckCollisionSAT(red_ball.GetPolygon(), blue_ball.GetPolygon())) {
            red_ball.SetTransparent(true);
            blue_ball.SetTransparent(true);
        }
        else {

            red_ball.SetTransparent(false);
            blue_ball.SetTransparent(false);
        }

        /* Make render calls and call the renderer. */
        red_ball.OnRender();
        blue_ball.OnRender();
        pongApp.Render();

        /* Poll events and input, and run end frame procedures. */
        pongApp.EndFrame();
        red_ball.OnEndFrame();
        blue_ball.OnEndFrame();

    }


    return 0;
}