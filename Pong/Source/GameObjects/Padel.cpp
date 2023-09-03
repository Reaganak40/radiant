#include "Padel.h"

Padel::Padel(double xPos, double yPos)
	: m_sprite(Radiant::Rect(Radiant::Vec2d(xPos, yPos), PADEL_WIDTH, PADEL_HEIGHT)),
	m_translation((0, 0), (0, 0))
{
	using namespace Radiant;

	m_translation.SetMaxVelocity(Vec2d(500, 500));
	acceleration = 550;

	m_sprite_color = ColorType::Red;

	SetLeftControl(std::vector<InputState>{A_KEY_PRESS, A_KEY_DOWN});
	SetRightControl(std::vector<InputState>{D_KEY_PRESS, D_KEY_DOWN});
	SetUpControl(std::vector<InputState>{W_KEY_PRESS, W_KEY_DOWN});
	SetDownControl(std::vector<InputState>{S_KEY_PRESS, S_KEY_DOWN});
}

Padel::~Padel()
{
}

void Padel::OnUpdate(const float deltaTime)
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

void Padel::OnRender()
{
	using namespace Radiant;
	
	Renderer::Begin();
	Renderer::SetPolygonColor(m_sprite_color);
	Renderer::AddPolygon(m_sprite);
	Renderer::End();
}

void Padel::OnEndFrame()
{
	m_translation.OnEndFrame();
}

void Padel::SetSpriteColor(Radiant::Color nColor)
{
	m_sprite_color = nColor;
}

void Padel::SetTransparent(bool transparency)
{
	if (transparency) {
		m_sprite_color.SetAlpha(0.3f);
	}
	else {
		m_sprite_color.SetAlpha(1.0f);
	}
}

void Padel::SetLeftControl(const std::vector <Radiant::InputState>& conditions)
{
	left_cond = conditions;
}

void Padel::SetRightControl(const std::vector <Radiant::InputState>& conditions)
{
	right_cond = conditions;

}

void Padel::SetUpControl(const std::vector <Radiant::InputState>& conditions)
{
	up_cond = conditions;

}

void Padel::SetDownControl(const std::vector <Radiant::InputState>& conditions)
{
	down_cond = conditions;
}
