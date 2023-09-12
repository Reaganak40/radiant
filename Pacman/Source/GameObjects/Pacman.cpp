#include "Pacman.h"

using namespace rdt;
Pacman::Pacman(double xPos, double yPos)
{
	for (int i = 0; i < 4; i++) {
		m_lastMove[i] = false;
		m_onBorder[i] = false;
	}

	spawnPos.x = xPos;
	spawnPos.y = yPos;

	left_cond = std::vector<InputState>{ A_KEY_PRESS, A_KEY_DOWN };
	right_cond = std::vector<InputState>{ D_KEY_PRESS, D_KEY_DOWN };
	up_cond = std::vector<InputState>{ W_KEY_PRESS, W_KEY_DOWN };
	down_cond = std::vector<InputState>{ S_KEY_PRESS, S_KEY_DOWN };

	m_map = nullptr;
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

	Respawn();
}

void Pacman::OnRelease()
{
}

void Pacman::OnProcessInput(const float deltaTIme)
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	Vec2i mapCoords = m_map->GetMapCoordinates(location);
	Vec2d nVelocity = Physics::GetVelocity(GetRealmID(), m_model_ID);

	UpdateBorderCheck(mapCoords);

	m_lastMapCoords = mapCoords;

	if (Input::CheckKeyboardState(right_cond)) {
		nVelocity.x = PACMAN_SPEED;
	}

	else if (Input::CheckKeyboardState(left_cond)) {
		nVelocity.x = -PACMAN_SPEED;
	}

	if (Input::CheckKeyboardState(up_cond)) {
		nVelocity.y = PACMAN_SPEED;
	}
	else if (Input::CheckKeyboardState(down_cond)) {
		nVelocity.y = -PACMAN_SPEED;
	}

	if (nVelocity.x < 0) {
		if (m_onBorder[LEFT] && m_map->GetWorldCoordinates(mapCoords).x >= location.x) {
			nVelocity.x = 0;
		}
	} else if (nVelocity.x > 0) {
		if (m_onBorder[RIGHT] && m_map->GetWorldCoordinates(mapCoords).x <= location.x) {
			nVelocity.x = 0;
		}
	}


	if (nVelocity.y < 0) {
		if (m_onBorder[DOWN] && m_map->GetWorldCoordinates(mapCoords).y >= location.y) {
			nVelocity.y = 0;
		}
	} else if (nVelocity.y > 0) {
		if (m_onBorder[UP] && m_map->GetWorldCoordinates(mapCoords).y <= location.y) {
			nVelocity.y = 0;
		}
	}

	if (nVelocity.x != 0 && nVelocity.y != 0) {

		if (m_lastMove[UP] || m_lastMove[DOWN]) {
			nVelocity.y = 0;
		}
		else {
			nVelocity.x = 0;
		}
	}

	for (int i = 0; i < 4; i++) {
		m_lastMove[i] = false;
	}


	if (nVelocity.x > 0) {
		m_lastMove[RIGHT] = true;
		Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Right);

	} else if (nVelocity.x < 0) {
		m_lastMove[LEFT] = true;
		Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Left);
	}
	else if (nVelocity.y < 0) {
		m_lastMove[DOWN] = true;
		Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Down);
	}
	else if (nVelocity.y > 0) {
		m_lastMove[UP] = true;
		Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Up);
	}

	Physics::SetVelocity(GetRealmID(), m_model_ID, nVelocity);
}

void Pacman::OnFinalUpdate()
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();

	if (location.x < 0 - PACMAN_SPRITE_WIDTH) {
		Physics::SetPosition(GetRealmID(), m_model_ID, { SCREEN_WIDTH, location.y });
	} else if (location.x > SCREEN_WIDTH + PACMAN_SPRITE_WIDTH) {
		Physics::SetPosition(GetRealmID(), m_model_ID, { 0, location.y });
	}
	else {
		Vec2i mapCoords = m_map->GetMapCoordinates(location);
		Vec2d centered_coords = m_map->GetWorldCoordinates(m_lastMapCoords);

		if (m_lastMove[UP] && m_onBorder[UP]) {
			if (mapCoords.y < m_lastMapCoords.y || centered_coords.y < location.y) {
				Physics::SetPosition(GetRealmID(), m_model_ID, { location.x, centered_coords.y});
			}
		} else if (m_lastMove[DOWN] && m_onBorder[DOWN]) {
			if (mapCoords.y > m_lastMapCoords.y || centered_coords.y > location.y) {
				Physics::SetPosition(GetRealmID(), m_model_ID, { location.x, centered_coords.y });
			}
		}

		if (m_lastMove[LEFT] && m_onBorder[LEFT]) {
			if (mapCoords.x < m_lastMapCoords.x || centered_coords.x > location.x) {
				Physics::SetPosition(GetRealmID(), m_model_ID, { centered_coords.x, location.y });
			}
		} else if (m_lastMove[RIGHT] && m_onBorder[RIGHT]) {
			if (mapCoords.y < m_lastMapCoords.y || centered_coords.x < location.x) {
				Physics::SetPosition(GetRealmID(), m_model_ID, { centered_coords.x, location.y });
			}
		}
	}
}

void Pacman::OnRender()
{
	using namespace rdt;

	Renderer::Begin(PACMAN_LAYER);
	Renderer::SetPolygonTexture("pacman");
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}

void Pacman::AddMapPtr(Map* map)
{
	m_map = map;
}

void Pacman::Respawn()
{
	Vec2i mapCoords = m_map->GetMapCoordinates({PACMAN_SPAWN_X, PACMAN_SPAWN_Y});

	Physics::SetPosition(GetRealmID(), m_model_ID, { PACMAN_SPAWN_X, m_map->GetWorldCoordinates({12, 22}).y});
	Physics::SetVelocity(GetRealmID(), m_model_ID, { 0, 0 });
	Physics::SetRotation(GetRealmID(), m_model_ID, Utils::Radians_Right);
}

void Pacman::UpdateBorderCheck(const rdt::Vec2i& mapCoords)
{
	m_onBorder[UP] = !m_map->IsInMap(mapCoords.y - 1, mapCoords.x);
	m_onBorder[DOWN] = !m_map->IsInMap(mapCoords.y + 1, mapCoords.x);

	m_onBorder[LEFT] = !m_map->IsInMap(mapCoords.y, mapCoords.x - 1);
	m_onBorder[RIGHT] = !m_map->IsInMap(mapCoords.y, mapCoords.x + 1);
}
