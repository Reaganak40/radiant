#include "Pacman.h"

using namespace rdt;
Pacman::Pacman(double xPos, double yPos)
{

	spawnPos.x = xPos;
	spawnPos.y = yPos;

	left_cond = std::vector<InputState>{A_KEY_PRESS, A_KEY_DOWN};
	right_cond = std::vector<InputState>{D_KEY_PRESS, D_KEY_DOWN};
	up_cond = std::vector<InputState>{W_KEY_PRESS, W_KEY_DOWN};
	down_cond = std::vector<InputState>{S_KEY_PRESS, S_KEY_DOWN};
}

Pacman::~Pacman()
{
}

void Pacman::OnBind()
{
	std::shared_ptr<Rect> sprite;
	m_model_ID = Physics::CreateObject(GetRealmID(), sprite = std::make_shared<Rect>(spawnPos, PACMAN_SPRITE_WIDTH, PACMAN_SPRITE_WIDTH));

	sprite->ApplyRotationOffset(M_PI);
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d(700, 700));
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppBouncy);
}

void Pacman::OnRelease()
{
}

void Pacman::OnProcessInput(const float deltaTIme)
{
	Physics::SetVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());

	if (Input::CheckKeyboardState(right_cond)) {
		Physics::SetVelocity(GetRealmID(), m_model_ID, { 300, 0 });
		Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Right);
	}

	else if (Input::CheckKeyboardState(left_cond)) {
		Physics::SetVelocity(GetRealmID(), m_model_ID, { -300, 0 });
		Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Left);
	}

	else if (Input::CheckKeyboardState(up_cond)) {
		Physics::SetVelocity(GetRealmID(), m_model_ID, { 0, 300 });
		Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Up);

	}

	else if (Input::CheckKeyboardState(down_cond)) {
		Physics::SetVelocity(GetRealmID(), m_model_ID, { 0, -300 });
		Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Down);
	}
}

void Pacman::OnFinalUpdate()
{
}

void Pacman::OnRender()
{
	using namespace rdt;

	Renderer::Begin(0);
	Renderer::SetPolygonTexture("pacman");
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}
