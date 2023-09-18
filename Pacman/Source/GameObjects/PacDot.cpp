#include "PacDot.h"

using namespace rdt;

PacDot::PacDot(double xPos, double yPos)
	: m_blinking_timer(0.35f)
{
	spawnPos = { xPos, yPos };
	m_color = rdt::WHITE;
	m_power_dot = false;
	m_show = true;
}

PacDot::~PacDot()
{
}

void PacDot::OnBind()
{
	int extra_size = 0;
	if (m_power_dot) {
		extra_size = 10;
		m_blinking_timer.Start();
	}

	AddObjectToWorld(std::make_shared<Rect>(spawnPos, PAC_DOT_WIDTH + extra_size, PAC_DOT_WIDTH + extra_size));
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, NoCollision);
	Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());

	Reset();
}

void PacDot::OnRelease()
{
}

void PacDot::OnProcessInput(const float deltaTIme)
{
	if (m_blinking_timer.IsRunning()) {
		
		if (m_blinking_timer.Update(deltaTIme)) {
			m_show = !m_show;
			m_blinking_timer.Start();
		}
	}
}

void PacDot::OnFinalUpdate()
{
}

void PacDot::OnRender()
{
	using namespace rdt;

	if (!m_eaten && m_show) {

		Renderer::Begin(PAC_DOT_LAYER);
		
		if (m_power_dot) {
			Renderer::SetPolygonTexture("powerdot");
		}
		else {
			Renderer::SetPolygonColor(m_color);
		}

		Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
		Renderer::End();
	}
}

void PacDot::SetColor(rdt::Color nColor)
{
	m_color = nColor;
}

bool PacDot::IsEaten()
{
	return m_eaten;
}

void PacDot::Eat()
{
	m_eaten = true;
}

void PacDot::Reset()
{
	m_eaten = false;
}

void PacDot::MakePowerDot()
{
	m_power_dot = true;
}

bool PacDot::IsPowerDot()
{
	return m_power_dot;
}

bool PacDot::ShouldEat(const rdt::Vec2d& suspect_position)
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	double error = TILE_WIDTH / 3;

	if (abs(suspect_position.x - location.x) < error && abs(suspect_position.y - location.y) < error) {
		return true;
	}
	return false;
}
