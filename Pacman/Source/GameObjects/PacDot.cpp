#include "PacDot.h"

PacDot::PacDot(double xPos, double yPos)
{
	spawnPos = { xPos, yPos };
}

PacDot::~PacDot()
{
}

void PacDot::OnBind()
{
	using namespace rdt;

	m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, PAC_DOT_WIDTH, PAC_DOT_WIDTH));

	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppRigid);
	Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
}

void PacDot::OnRelease()
{
}

void PacDot::OnProcessInput(const float deltaTIme)
{
}

void PacDot::OnFinalUpdate()
{
}

void PacDot::OnRender()
{
	using namespace rdt;

	Renderer::Begin(PAC_DOT_LAYER);
	Renderer::SetPolygonColor(WHITE);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}
