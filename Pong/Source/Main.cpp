#include "Radiant.h"

using namespace Radiant;

int main(void)
{
    Renderer::Initialize();
    Renderer::CreateWindow("Pong", 1280, 720);

    Rect shape(Vec2d(100, 200.0), 100.0, 100.0);


    /* Loop until the user closes the window */
    while (!Renderer::ShouldWindowClose())
    {
        Renderer::Clear();

        Renderer::Begin();
        Renderer::SetPolygonColor(ColorType::Green);
        Renderer::AddPolygon(shape);
        Renderer::End();


        Renderer::Draw();
    }

    Renderer::Destroy();

    return 0;
}