#include "Pacman.h"

using namespace rdt;
Pacman::Pacman(double xPos, double yPos)
{

	spawnPos.x = xPos;
	spawnPos.y = yPos;

	left_cond = std::vector<InputState>{ A_KEY_PRESS, A_KEY_DOWN };
	right_cond = std::vector<InputState>{ D_KEY_PRESS, D_KEY_DOWN };
	up_cond = std::vector<InputState>{ W_KEY_PRESS, W_KEY_DOWN };
	down_cond = std::vector<InputState>{ S_KEY_PRESS, S_KEY_DOWN };
}

Pacman::~Pacman()
{
}

void Pacman::OnBind()
{
	std::shared_ptr<Rect> sprite;
	m_model_ID = Physics::CreateObject(GetRealmID(), sprite = std::make_shared<Rect>(spawnPos, PACMAN_SPRITE_WIDTH, PACMAN_SPRITE_WIDTH));

	sprite->ApplyRotationOffset(M_PI);
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppRigid);
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetFriction(GetRealmID(), m_model_ID, 0);

	// start by moving right
	Physics::SetVelocity(GetRealmID(), m_model_ID, { 300, 0 });
	Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Right);
}

void Pacman::OnRelease()
{
}

void Pacman::OnProcessInput(const float deltaTIme)
{

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
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();

	if (location.x < 0 - PACMAN_SPRITE_WIDTH) {
		Physics::SetPosition(GetRealmID(), m_model_ID, { SCREEN_WIDTH, location.y });
	} else if (location.x > SCREEN_WIDTH + PACMAN_SPRITE_WIDTH) {
		Physics::SetPosition(GetRealmID(), m_model_ID, { 0, location.y });
	}
}

void Pacman::OnRender()
{
	using namespace rdt;

	Renderer::Begin(0);
	Renderer::SetPolygonTexture("pacman");
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}
