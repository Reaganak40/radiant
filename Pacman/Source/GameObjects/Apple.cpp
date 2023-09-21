#include "Apple.h"

using namespace rdt;
Apple::Apple()
{
	m_texName = "apple";
}

Apple::~Apple()
{
}

void Apple::OnBind()
{
	AddObjectToWorld(std::make_shared<Rect>(Vec2d( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 ), 200, 200));

	Physics::SetObjectProperties(GetRealmID(), m_model_ID, DontResolve);
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetFriction(GetRealmID(), m_model_ID, 0);
}

void Apple::OnRelease()
{
}

void Apple::OnProcessInput(const float deltaTIme)
{
	if (m_texName == "apple") {
		Physics::SetVelocity(GetRealmID(), m_model_ID, { 0, 0 });
	}
	else {
		Physics::SetVelocity(GetRealmID(), m_model_ID, { 0, -20 });
	}
}

void Apple::OnFinalUpdate()
{
}

void Apple::OnRender()
{
	Renderer::Begin();
	//Renderer::SetPolygonTexture(m_texName);

	if (m_texName == "apple") {
	}
	Renderer::SetPolygonColor(Color(0.0, 0.8f, 0.0f, 1.0f));

	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}

void Apple::OnMessage(rdt::Message msg)
{
}

void Apple::SetTexture(const std::string& texName)
{
	m_texName = texName;
}

void Apple::MoveTo(rdt::Vec2d nPos)
{
	Physics::SetPosition(GetRealmID(), m_model_ID, nPos);
}
