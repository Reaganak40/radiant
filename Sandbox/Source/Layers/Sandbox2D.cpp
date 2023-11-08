#include "Sandbox2D.h"

using namespace rdt;

Sandbox2D::Sandbox2D()
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	BindAll();
}

void Sandbox2D::OnRender()
{
	Layer::OnRender();
}
