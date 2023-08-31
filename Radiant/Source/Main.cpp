#include "pch.h"
#include "Renderer/Renderer.h"

int main(void)
{
    Renderer::Initialize();
    Renderer::CreateWindow("Welcome to Radiant", 1280, 720);

    /* Loop until the user closes the window */
    while (!Renderer::ShouldWindowClose())
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer::Draw();
        
    }

    Renderer::Destroy();

    return 0;
}