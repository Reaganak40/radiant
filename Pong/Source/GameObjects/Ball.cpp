#include "Ball.h"

Ball::Ball(double xPos, double yPos)
	: m_sprite(Radiant::Rect(Radiant::Vec2d(xPos, yPos), 100.0f, 100.0f)),
	m_translation((0, 0), (0, 0))
{
	using namespace Radiant;

	m_translation.SetMaxVelocity(Vec2d(300, 300));
	acceleration = 250;

	m_sprite_color = ColorType::Red;

	SetLeftControl(std::vector<InputState>{A_KEY_PRESS, A_KEY_DOWN});
	SetRightControl(std::vector<InputState>{D_KEY_PRESS, D_KEY_DOWN});
	SetUpControl(std::vector<InputState>{W_KEY_PRESS, W_KEY_DOWN});
	SetDownControl(std::vector<InputState>{S_KEY_PRESS, S_KEY_DOWN});
}

Ball::~Ball()
{
}

void Ball::OnUpdate(const float deltaTime)
{
	using namespace Radiant;
	
	m_translation.SetAcceleration(Vec2d(0, 0));

	if (Input::CheckKeyboardState(right_cond)) {
		m_translation.SetAccelerationX(acceleration);
	}

	if (Input::CheckKeyboardState(left_cond)) {
		m_translation.SetAccelerationX(-acceleration);
	}

	if (Input::CheckKeyboardState(up_cond)) {
		m_translation.SetAccelerationY(acceleration);
	}

	if (Input::CheckKeyboardState(down_cond)) {
		m_translation.SetAccelerationY(-acceleration);
	}

	m_translation.UpdateVelocity(deltaTime);
	m_translation.Translate(m_sprite, deltaTime);
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
	m_translation.OnEndFrame();
}

void Ball::SetSpriteColor(Radiant::Color nColor)
{
	m_sprite_color = nColor;
}

void Ball::SetTransparent(bool transparency)
{
	if (transparency) {
		m_sprite_color.SetAlpha(0.3f);
	}
	else {
		m_sprite_color.SetAlpha(1.0f);
	}
}

void Ball::SetLeftControl(const std::vector <Radiant::InputState>& conditions)
{
	left_cond = conditions;
}

void Ball::SetRightControl(const std::vector <Radiant::InputState>& conditions)
{
	right_cond = conditions;

}

void Ball::SetUpControl(const std::vector <Radiant::InputState>& conditions)
{
	up_cond = conditions;

}

void Ball::SetDownControl(const std::vector <Radiant::InputState>& conditions)
{
	down_cond = conditions;
}
