#include "Pong.h"
#include "GameObjects/Ball.h"

using namespace Radiant;

int main(void)
{
    Application pongApp;
    pongApp.Start("Pong");

    Ball ball(300.0, 300.0);

    Renderer::AttachGui(new DiagnosticsGUI);

    /* Loop until the user closes the window */
    while (pongApp.IsRunning())
    {
        pongApp.Update();
        ball.OnUpdate(pongApp.GetDeltaTime());

        ball.OnRender();
        pongApp.Render();

        pongApp.EndFrame();
        ball.OnEndFrame();

    }


    return 0;
}