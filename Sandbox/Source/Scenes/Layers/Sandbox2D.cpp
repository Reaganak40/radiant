#include "Sandbox2D.h"

using namespace rdt;

Sandbox2D::Sandbox2D()
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnRender()
{
	Renderer::DrawRect({ 300, 300 }, { 200, 200 }, BLUE, 0);

	RunRenderQueue();
}
