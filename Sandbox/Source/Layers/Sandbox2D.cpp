#include "Sandbox2D.h"
#include "GameObjects/TestQuad.h"

using namespace rdt;

Sandbox2D::Sandbox2D()
{
	CreateNewRealm();
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	BindAll();

	Physics::ActivateRealm(GetRealms()[0]);
}

void Sandbox2D::OnRender()
{
	Layer::OnRender();
}
