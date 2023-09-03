#include "Ball.h"

Ball::Ball(double xPos, double yPos)
	: m_sprite(Radiant::Rect(Radiant::Vec2d(xPos, yPos), BALL_RADIUS, BALL_RADIUS))
{
	m_sprite_color = Radiant::ColorType::Green;
}

Ball::~Ball()
{
}

void Ball::OnUpdate(const float deltaTIme)
{
}

void Ball::OnRender()
{
	using namespace Radiant;
	Renderer::Begin();
	Renderer::SetPolygonColor(m_sprite_color);
	Renderer::AddPolygon(m_sprite);
	Renderer::End();
}

void Ball::OnEndFrame()
{
}
