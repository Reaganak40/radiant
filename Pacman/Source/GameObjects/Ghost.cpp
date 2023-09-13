#include "Ghost.h"

using namespace rdt;

Ghost::Ghost(GhostName nName)
{
	m_name = nName;
	spawnPos = Vec2d::Zero();
	m_map = nullptr;

	switch (nName) {
	case BLINKY:
		m_frame_row = 0;
		m_target_coords = { 13, 4 };
		m_direction = PacmanMoveDirection::LEFT;
		m_speed = GHOST_SPEED + 50;
		break;
	case PINKY:
		m_target_coords = { 16, 4 };
		m_frame_row = 1;
		m_direction = PacmanMoveDirection::LEFT;
		m_speed = GHOST_SPEED;
		break;
	case INKY:
		m_target_coords = { 19, 4 };
		m_frame_row = 2;
		m_direction = PacmanMoveDirection::RIGHT;
		m_speed = GHOST_SPEED;
		break;
	case CLYDE:
		m_target_coords = { 22, 4 };
		m_frame_row = 3;
		m_direction = PacmanMoveDirection::RIGHT;
		m_speed = GHOST_SPEED;
		break;
	default:
		m_frame_row = 0;
		m_direction = PacmanMoveDirection::RIGHT;
		m_speed = 0;
		break;
	}
	m_frame_col = 0;

}

Ghost::~Ghost()
{
}

void Ghost::OnBind()
{
	spawnPos = m_map->GetWorldCoordinates(m_target_coords);
	m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, GHOST_SPRITE_WIDTH, GHOST_SPRITE_WIDTH));

	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppRigid);
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetFriction(GetRealmID(), m_model_ID, 0);
}

void Ghost::OnRelease()
{
}

void Ghost::OnProcessInput(const float deltaTIme)
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	const Vec2d target = m_map->GetWorldCoordinates(m_target_coords);
	Vec2d nVelocity = Vec2d::Zero();

	if (location == target) {
		SelectNewTarget();
	}

	switch (m_direction) {
	case LEFT:
		nVelocity.x = -m_speed;
		break;
	case RIGHT:
		nVelocity.x = m_speed;
		break;
	case UP:
		nVelocity.y = m_speed;
		break;
	case DOWN:
		nVelocity.y = -m_speed;
		break;
	}

	Physics::SetVelocity(GetRealmID(), m_model_ID, nVelocity);
}

void Ghost::OnFinalUpdate()
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	const Vec2d worldCoords = m_map->GetWorldCoordinates(m_target_coords);
	double error = TILE_WIDTH / 4;

	switch (m_direction) {
	case LEFT:
		if (location.x < worldCoords.x + error) {
			Physics::SetPosition(GetRealmID(), m_model_ID, worldCoords);
		}
		break;
	case RIGHT:
		if (location.x > worldCoords.x - error) {
			Physics::SetPosition(GetRealmID(), m_model_ID, worldCoords);
		}
		break;
	case UP:
		if (location.y > worldCoords.y - error) {
			Physics::SetPosition(GetRealmID(), m_model_ID, worldCoords);
		}
		break;
	case DOWN:
		if (location.y < worldCoords.y + error) {
			Physics::SetPosition(GetRealmID(), m_model_ID, worldCoords);
		}
		break;
	}
}

void Ghost::OnRender()
{
	Renderer::Begin(GHOST_LAYER);
	Renderer::SetPolygonTexture("ghost", m_frame_col, m_frame_row);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}

void Ghost::AddMapPtr(Map* nMap)
{
	m_map = nMap;
}

void Ghost::SelectNewTarget()
{
	bool options[4];

	/* Check if ghost is using tunnel to teleport. */
	if (m_target_coords.x == 0) {
		m_target_coords.x = 31;
		Physics::SetPosition(GetRealmID(), m_model_ID, m_map->GetWorldCoordinates(m_target_coords));
		m_target_coords.x -= 1;
		m_direction = PacmanMoveDirection::LEFT;
		return;
	}
	else if (m_target_coords.x == 31) {
		m_target_coords.x = 0;
		Physics::SetPosition(GetRealmID(), m_model_ID, m_map->GetWorldCoordinates(m_target_coords));
		m_target_coords.x += 1;
		m_direction = PacmanMoveDirection::RIGHT;
		return;
	}

	/* Check what legal directions exist. */
	if (m_direction == DOWN) {
		options[UP] = false;
	}
	else {
		options[UP] = m_map->IsInMap(m_target_coords.y - 1, m_target_coords.x);
	}

	if (m_direction == UP) {
		options[DOWN] = false;
	}
	else {
		options[DOWN] = m_map->IsInMap(m_target_coords.y + 1, m_target_coords.x);
	}

	if (m_direction == RIGHT) {
		options[LEFT] = false;
	}
	else {
		options[LEFT] = m_map->IsInMap(m_target_coords.y, m_target_coords.x - 1);
	}

	if (m_direction == LEFT) {
		options[RIGHT] = false;
	}
	else {
		options[RIGHT] = m_map->IsInMap(m_target_coords.y, m_target_coords.x + 1);
	}


	/* Choose randomly the next direction. */
	int optionCount = 0;
	for (int i = 0; i < 4; i++) {
		if (options[i]) {
			optionCount++;
		}
	}
	int choiceIndex = Utils::RandInt(1, optionCount);

	for (int i = 0; i < 4; i++) {
		if (options[i]) {
			choiceIndex--;

			if (choiceIndex == 0) {

				switch (i) {
				case UP:
					m_direction = PacmanMoveDirection::UP;
					m_target_coords.y -= 1;
					break;
				case DOWN:
					m_direction = PacmanMoveDirection::DOWN;
					m_target_coords.y += 1;
					break;
				case LEFT:
					m_direction = PacmanMoveDirection::LEFT;
					m_target_coords.x -= 1;
					break;
				case RIGHT:
					m_direction = PacmanMoveDirection::RIGHT;
					m_target_coords.x += 1;
					break;
				default:
					m_direction = PacmanMoveDirection::UP;
					break;
				}
				return;
			}
		}
	}
}

