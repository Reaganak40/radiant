#include "Pong.h"

using namespace Radiant;

int main(void)
{
    Application pongApp;
    pongApp.Start("Pong");

    Rect shape(Vec2d(100, 200.0), 100.0, 100.0);

    Renderer::AttachGui(new DiagnosticsGUI);

    /* Loop until the user closes the window */
    while (pongApp.IsRunning())
    {
        pongApp.Update();

        Renderer::Begin();
        Renderer::SetPolygonColor(ColorType::Green);
        Renderer::AddPolygon(shape);
        Renderer::End();

        pongApp.Render();

    }


    return 0;
}