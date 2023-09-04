#include "Padel.h"

Padel::Padel(double xPos, double yPos)
{
	using namespace Radiant;

	spawnPos = Vec2d(xPos, yPos);
	m_sprite_color = RED;

	SetLeftControl(std::vector<InputState>{A_KEY_PRESS, A_KEY_DOWN});
	SetRightControl(std::vector<InputState>{D_KEY_PRESS, D_KEY_DOWN});
	SetUpControl(std::vector<InputState>{W_KEY_PRESS, W_KEY_DOWN});
	SetDownControl(std::vector<InputState>{S_KEY_PRESS, S_KEY_DOWN});
}

Padel::~Padel()
{
}

void Padel::OnRegister()
{
	using namespace Radiant;
	m_model_ID = Physics::CreateObject(Rect(spawnPos, PADEL_WIDTH, PADEL_HEIGHT));
}

void Padel::OnProcessInput(const float deltaTIme)
{
}

void Padel::OnFinalUpdate()
{
}

void Padel::OnRender()
{
	using namespace Radiant;
	
	Renderer::Begin();
	Renderer::SetPolygonColor(m_sprite_color);
	Renderer::AddPolygon(Physics::GetPolygon(m_model_ID));
	Renderer::End();
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
