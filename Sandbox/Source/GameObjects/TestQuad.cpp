#include "TestQuad.h"

using namespace rdt;

TestQuad::TestQuad(const std::string& alias, Vec2d spawnPos)
{
	RegisterToMessageBus(alias);
	m_spawnPos = spawnPos;
}

TestQuad::~TestQuad()
{
}

void TestQuad::OnBind()
{
	AddObjectToWorld(std::make_shared<Rect>(m_spawnPos, 300, 300));
	Physics::SetObjectProperties(GetRealmID(), GetModelID(), DontResolve);
	Physics::SetMaximumVelocity(GetRealmID(), GetModelID(), Vec2d::Zero());
	Physics::SetAcceleration(GetRealmID(), GetModelID(), Vec2d::Zero());
}

void TestQuad::OnRender()
{
	return;
}
