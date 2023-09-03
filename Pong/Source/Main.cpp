#include "Pong.h"
#include "GameObjects/Padel.h"
#include "GameObjects/Ball.h"

using namespace Radiant;

int main(void)
{
    Application pongApp;
    pongApp.Start("Pong");

    Padel RedPlayer(10.0 + (PADEL_WIDTH / 2), 300.0);
    RedPlayer.SetLeftControl({});
    RedPlayer.SetRightControl({});

    Padel BluePlayer(pongApp.WindowWidth() - (PADEL_WIDTH / 2) - 10, 300.0);
    BluePlayer.SetSpriteColor(Color(ColorType::Blue));
    BluePlayer.SetLeftControl({});
    BluePlayer.SetRightControl({});
    BluePlayer.SetUpControl(std::vector<InputState>{I_KEY_PRESS, I_KEY_DOWN});
    BluePlayer.SetDownControl(std::vector<InputState>{K_KEY_PRESS, K_KEY_DOWN});

    Ball ball(pongApp.WindowWidth() / 2, pongApp.WindowHeight() / 2);

    Renderer::AttachGui(new DiagnosticsGUI);

    /* Loop until the user closes the window */
    while (pongApp.IsRunning())
    {
        /* Update App and run update procedures. */
        pongApp.Update();
        RedPlayer.OnUpdate(pongApp.GetDeltaTime());
        BluePlayer.OnUpdate(pongApp.GetDeltaTime());

        /* Make render calls and call the renderer. */
        RedPlayer.OnRender();
        BluePlayer.OnRender();
        ball.OnRender();
        pongApp.Render();

        /* Poll events and input, and run end frame procedures. */
        pongApp.EndFrame();
        RedPlayer.OnEndFrame();
        BluePlayer.OnEndFrame();
    }


    return 0;
}