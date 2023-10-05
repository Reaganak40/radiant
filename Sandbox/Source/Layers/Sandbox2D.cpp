#include "Sandbox2D.h"
#include "GameObjects/TestQuad.h"

using namespace rdt;

Sandbox2D::Sandbox2D()
{
	CreateNewRealm();

	TestQuad* quad1;
	m_game_objects.push_back(quad1 = new TestQuad("quad1"));
	quad1->RegisterToRealm(m_realms[0]);
	quad1->RegisterToLayer(GetID());
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	for (auto& object : m_game_objects) {
		object->OnBind();
	}

	Physics::ActivateRealm(m_realms[0]);
}

void Sandbox2D::OnRender()
{
	RunRenderQueue();
}
