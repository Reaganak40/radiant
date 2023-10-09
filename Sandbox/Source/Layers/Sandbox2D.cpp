#include "Sandbox2D.h"
#include "GameObjects/TestQuad.h"

using namespace rdt;

Sandbox2D::Sandbox2D()
{
	CreateNewRealm();

	TestQuad* quad1;
	RegisterGameObject(quad1 = new TestQuad("quad1", { 160, 160 }));
	quad1->RegisterToRealm(GetRealms()[0]);
	quad1->RegisterToLayer(GetID());

	TestQuad* quad2;
	RegisterGameObject(quad2 = new TestQuad("quad2", { 700, 400 }));
	quad2->RegisterToRealm(GetRealms()[0]);
	quad2->RegisterToLayer(GetID());
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
