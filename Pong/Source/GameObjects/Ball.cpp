#include "Ball.h"

Ball::Ball(double xPos, double yPos)
	: m_timer(1.5)
{
	using namespace Radiant;

	spawnPos = Vec2d(xPos, yPos);
	m_sprite_color = GREEN;
	acceleration = 500;

	m_score[0] = 0;
	m_score[1] = 0;
}

Ball::~Ball()
{
}

void Ball::OnBind()
{
	using namespace Radiant;
	
	m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, BALL_RADIUS * 2, BALL_RADIUS * 2));

	
	Physics::SetFriction(GetRealmID(), m_model_ID, 0);
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppBouncy);
	
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

	auto ballX = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin().x;

	if (ballX < -100 || ballX > WORLD_WIDTH + 100) {

		if (ballX < -100) {
			m_score[1]++;
		}
		else {
			m_score[0]++;
		}

		SetUpBall();
		m_timer.Start();
	}

}

void Ball::OnRender()
{
	using namespace Radiant;

	Renderer::Begin(BALL_LAYER);
	Renderer::SetPolygonColor(m_sprite_color);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();

}

void Ball::SetUpBall()
{
	using namespace Radiant;

	Physics::SetPosition(GetRealmID(), m_model_ID, Vec2d(WORLD_WIDTH / 2, WORLD_HEIGHT / 2));
	Physics::SetVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetAccelerationX(GetRealmID(), m_model_ID, 0);
}

void Ball::StartMovingBall()
{
	using namespace Radiant;

	int direction = Utils::RandInt(0, 1) == 0 ? -1 : 1;
	Physics::SetVelocity(GetRealmID(), m_model_ID, Vec2d(
		 direction * BALL_START_SPEED,
		Utils::RandInt(0, 1) == 0 ? Utils::RandomFloat(100, 450) : -Utils::RandomFloat(100, 450)
	));
	Physics::SetAccelerationX(GetRealmID(), m_model_ID, direction * 40);

}

void Ball::GetScore(unsigned int& player1, unsigned int& player2)
{
	player1 = m_score[0];
	player2 = m_score[1];
}

void Ball::Reset()
{
	m_score[0] = 0;
	m_score[1] = 0;

	SetUpBall();
	m_timer.Start();
}
