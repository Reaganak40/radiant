#include "PacDot.h"

PacDot::PacDot(double xPos, double yPos)
{
	spawnPos = { xPos, yPos };
	m_color = rdt::WHITE;
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

	Reset();
}

void PacDot::OnRelease()
{
}

void PacDot::OnProcessInput(const float deltaTIme)
{
	m_color = rdt::WHITE;
}

void PacDot::OnFinalUpdate()
{
}

void PacDot::OnRender()
{
	using namespace rdt;

	if (!m_eaten) {
		Renderer::Begin(PAC_DOT_LAYER);
		Renderer::SetPolygonColor(m_color);
		Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
		Renderer::End();
	}
}

void PacDot::SetColor(rdt::Color nColor)
{
	m_color = nColor;
}

void PacDot::Eat()
{
	m_eaten = true;
}

void PacDot::Reset()
{
	m_eaten = false;
}
