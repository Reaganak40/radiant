#include "Ball.h"

Ball::Ball(double xPos, double yPos)
	: m_sprite(Radiant::Rect(Radiant::Vec2d(xPos, yPos), 100.0f, 100.0f)),
	m_translation((0, 0), (0, 0))
{
	m_translation.SetMaxVelocity(Radiant::Vec2d(300, 300));
	acceleration = 250;
}

Ball::~Ball()
{
}

void Ball::OnUpdate(const float deltaTime)
{
	using namespace Radiant;
	
	m_translation.SetAcceleration(Vec2d(0, 0));

	if (Input::CheckKeyboardState(D_KEY_PRESS | D_KEY_DOWN)) {
		m_translation.SetAccelerationX(acceleration);
	}

	if (Input::CheckKeyboardState(A_KEY_PRESS | A_KEY_DOWN)) {
		m_translation.SetAccelerationX(-acceleration);
	}

	if (Input::CheckKeyboardState(W_KEY_PRESS | W_KEY_DOWN)) {
		m_translation.SetAccelerationY(acceleration);
	}

	if (Input::CheckKeyboardState(S_KEY_PRESS | S_KEY_DOWN)) {
		m_translation.SetAccelerationY(-acceleration);
	}

	m_translation.UpdateVelocity(deltaTime);
	m_translation.Translate(m_sprite, deltaTime);
}

void Ball::OnRender()
{
	using namespace Radiant;
	
	Renderer::Begin();
	Renderer::SetPolygonColor(ColorType::Red);
	Renderer::AddPolygon(m_sprite);
	Renderer::End();
}

void Ball::OnEndFrame()
{
	m_translation.OnEndFrame();
}
