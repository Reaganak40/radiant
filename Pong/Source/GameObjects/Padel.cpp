#include "Padel.h"

Padel::Padel(double xPos, double yPos)
	: m_fill_sprite(true), m_AI(false), m_ball(nullptr)
{
	using namespace Radiant;

	spawnPos = Vec2d(xPos, yPos);
	m_sprite_color = WHITE;
	acceleration = PLAYER_ACCELERATION;

	SetLeftControl(std::vector<InputState>{A_KEY_PRESS, A_KEY_DOWN});
	SetRightControl(std::vector<InputState>{D_KEY_PRESS, D_KEY_DOWN});
	SetUpControl(std::vector<InputState>{W_KEY_PRESS, W_KEY_DOWN});
	SetDownControl(std::vector<InputState>{S_KEY_PRESS, S_KEY_DOWN});
}

Padel::~Padel()
{
}

void Padel::OnBind()
{
	using namespace Radiant;
	m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, PADEL_WIDTH, PADEL_HEIGHT));

	Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d(700, 700));
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppBouncy);
}

void Padel::OnRelease()
{
	using namespace Radiant;
	Physics::SetPosition(GetRealmID(), m_model_ID, spawnPos);
	Physics::SetVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
}

void Padel::OnProcessInput(const float deltaTime)
{
	using namespace Radiant;

	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	
	if (m_AI) {
		OnAIUpdate(deltaTime);
	} else {

		if (Input::CheckKeyboardState(right_cond)) {
			Physics::SetAccelerationX(GetRealmID(), m_model_ID, acceleration);
		}

		if (Input::CheckKeyboardState(left_cond)) {
			Physics::SetAccelerationX(GetRealmID(), m_model_ID, -acceleration);
		}

		if (Input::CheckKeyboardState(up_cond)) {
			Physics::SetAccelerationY(GetRealmID(), m_model_ID, acceleration);
		}

		if (Input::CheckKeyboardState(down_cond)) {
			Physics::SetAccelerationY(GetRealmID(), m_model_ID, -acceleration);
		}
	}
}

void Padel::OnFinalUpdate()
{
}

void Padel::OnRender()
{
	using namespace Radiant;
	
	Renderer::Begin(0);
	if (!m_fill_sprite) {
		Renderer::SetRenderCond(DrawOutline);
	}
	Renderer::SetPolygonColor(m_sprite_color);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
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

void Padel::FillSprite(bool fill)
{
	m_fill_sprite = fill;
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

Radiant::Vec2d Padel::GetPosition()
{
	return Radiant::Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
}

void Padel::SetComputerControl(bool isComputer)
{
	m_AI = isComputer;

	if (m_AI) {
		m_sprite_color = Radiant::RED;
		acceleration = AI_ACCELERATION;
	}
	else {
		m_sprite_color = Radiant::WHITE;
		acceleration = PLAYER_ACCELERATION;
	}
}

void Padel::AddBallReference(Ball* ball)
{
	m_ball = ball;
}

void Padel::OnAIUpdate(const float deltaTime)
{
	if (m_ball == nullptr) {
		return;
	}

	using namespace Radiant;


	if (Physics::GetVelocity(GetRealmID(), m_ball->m_model_ID).x < 0) {
		return;
	}

	if (Physics::GetPolygon(GetRealmID(), m_ball->m_model_ID).GetOrigin().y >
		Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin().y) {

		Physics::SetAccelerationY(GetRealmID(), m_model_ID, acceleration);

	}
	else {
		Physics::SetAccelerationY(GetRealmID(), m_model_ID, -acceleration);

	}
}
