#include "Ball.h"

Ball::Ball(double xPos, double yPos)
	: m_timer(1.5)
{
	using namespace Radiant;

	spawnPos = Vec2d(xPos, yPos);
	m_sprite_color = GREEN;
	acceleration = 500;

}

Ball::~Ball()
{
}

void Ball::OnRegister()
{
	using namespace Radiant;
	
	m_model_ID = Physics::CreateObject(std::make_shared<Rect>(spawnPos, BALL_RADIUS/2, BALL_RADIUS/2));

	
	Physics::SetFriction(m_model_ID, 0);
	Physics::SetObjectProperties(m_model_ID, ppBouncy);
	
	SetUpBall();
	m_timer.Start();
}

void Ball::OnProcessInput(const float deltaTIme)
{
	using namespace Radiant;

	if (m_timer.IsRunning()) {
		
		if (m_timer.Update(deltaTIme)) {
			m_timer.End();
			StartMovingBall();
		}
	}
}

void Ball::OnFinalUpdate()
{
	using namespace Radiant;
	if (Physics::GetPolygon(m_model_ID).GetOrigin().x < -100
		|| Physics::GetPolygon(m_model_ID).GetOrigin().x > Renderer::GetWindowWidth() + 100) {

		SetUpBall();
		m_timer.Start();
	}

}

void Ball::OnRender()
{
	using namespace Radiant;

	Renderer::Begin(BALL_LAYER);
	Renderer::SetPolygonColor(m_sprite_color);
	Renderer::AddPolygon(Physics::GetPolygon(m_model_ID));
	Renderer::End();

}

void Ball::SetUpBall()
{
	using namespace Radiant;

	Physics::SetPosition(m_model_ID, Vec2d(Renderer::GetWindowWidth() / 2, Renderer::GetWindowHeight() / 2));
	Physics::SetVelocity(m_model_ID, Vec2d::Zero());
	Physics::SetAccelerationX(m_model_ID, 0);
}

void Ball::StartMovingBall()
{
	using namespace Radiant;

	int direction = Utils::RandInt(0, 1) == 0 ? -1 : 1;
	Physics::SetVelocity(m_model_ID, Vec2d(
		 direction * 450.0,
		Utils::RandInt(0, 1) == 0 ? Utils::RandomFloat(100, 450) : -Utils::RandomFloat(100, 450)
	));
	Physics::SetAccelerationX(m_model_ID, direction * 40);

}
