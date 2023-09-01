#include "Radiant.h"

using namespace Radiant;

int main(void)
{
    Renderer::Initialize();
    Renderer::CreateWindow("Welcome to Radiant", 1280, 720);

    Triangle shape(Vec2d(100, 200.0), 100.0, 100.0);


    /* Loop until the user closes the window */
    while (!Renderer::ShouldWindowClose())
    {
        Renderer::Clear();

        Renderer::Begin();
        Renderer::AddPolygon(shape);
        Renderer::End();

        shape.Move(1.0, 0);

        Renderer::Draw();
    }

    Renderer::Destroy();

    return 0;
}