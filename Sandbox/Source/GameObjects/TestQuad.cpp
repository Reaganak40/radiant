#include "TestQuad.h"

using namespace rdt;

TestQuad::TestQuad(const std::string& alias)
{
	RegisterToMessageBus(alias);
}

TestQuad::~TestQuad()
{
}

void TestQuad::OnBind()
{
	AddObjectToWorld(std::make_shared<Rect>(Vec2d(160, 160), 300, 300));
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, DontResolve);
	Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
}

void TestQuad::OnRender()
{
	Renderer::Begin();
	Renderer::SetPolygonColor(GREEN);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}
