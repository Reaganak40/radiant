#include "Ghost.h"

using namespace rdt;

Ghost::Ghost(GhostName nName)
	: m_frame_timer(0.35), m_blink_timer(0.20)
{
	m_name = nName;
	spawnPos = Vec2d::Zero();
	m_map = nullptr;
	m_target_coords = { 15, 10 };
	m_is_vulnerable = false;

	switch (nName) {
	case BLINKY:
		m_frame_row = 0;
		m_direction = PacmanMoveDirection::LEFT;
		m_home_timer.SetInterval(0.5);
		break;
	case PINKY:
		m_frame_row = 1;
		m_direction = PacmanMoveDirection::DOWN;
		m_home_timer.SetInterval(Utils::RandomFloat(1.0, 3.0));

		break;
	case INKY:
		m_frame_row = 2;
		m_direction = PacmanMoveDirection::UP;
		m_home_timer.SetInterval(Utils::RandomFloat(5.0, 8.0));
		break;
	case CLYDE:
		m_frame_row = 3;
		m_direction = PacmanMoveDirection::UP;
		m_home_timer.SetInterval(Utils::RandomFloat(8.0, 15.0));
		break;
	default:
		m_frame_row = 0;
		m_direction = PacmanMoveDirection::RIGHT;
		m_speed = 0;
		break;
	}

	if (m_name == BLINKY) {
		m_is_home = false;
		m_speed = GHOST_SPEED + 50;
	}
	else {
		m_is_home = true;
		m_speed = GHOST_SPEED;
	}

	m_frame_col = 0;
	df = 1;

	Look(m_direction);
}

Ghost::~Ghost()
{
}

void Ghost::OnBind()
{
	spawnPos = m_map->GetWorldCoordinates(m_target_coords);
	
	switch (m_name) {
	case BLINKY:
		m_target_coords.x -= 1;
		spawnPos.x = BLINKY_HOME_X;
		break;
	case PINKY:
		spawnPos.x = PINKY_HOME_X;
		spawnPos.y = GHOST_HOME_Y;
		break;
	case INKY:
		spawnPos.x = INKY_HOME_X;
		spawnPos.y = GHOST_HOME_Y;
		break;
	case CLYDE:
		spawnPos.y = GHOST_HOME_Y;
		spawnPos.x = CLYDE_HOME_X;
		break;
	default:
		break;
	}
	
	m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, GHOST_SPRITE_WIDTH, GHOST_SPRITE_WIDTH));

	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppRigid);
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetFriction(GetRealmID(), m_model_ID, 0);

	if (m_name == BLINKY) {
		Physics::SetVelocity(GetRealmID(), m_model_ID, {-m_speed, 0});
	}

	if (m_is_home) {
		m_home_timer.Start();
	}

	m_frame_timer.Start();
}

void Ghost::OnRelease()
{
}

void Ghost::OnProcessInput(const float deltaTime)
{
	
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	const Vec2d target = m_map->GetWorldCoordinates(m_target_coords);

	if (!m_is_home && location == target) {
		Vec2d nVelocity = Vec2d::Zero();
		SelectNewTarget();

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
		Look(m_direction);

		Physics::SetVelocity(GetRealmID(), m_model_ID, nVelocity);
	
	} else if (m_is_home) {
		Vec2d nVelocity = Vec2d::Zero();

		if (m_home_timer.IsRunning() && !m_home_timer.Update(deltaTime)) {

			if (m_direction == UP) {
				nVelocity.y = m_speed * 0.50;
			}
			else if (m_direction == DOWN) {
				nVelocity.y = -m_speed * 0.50;
			}

		}
		else {
			// time to leave base

			if (location.x > BLINKY_HOME_X) {
				nVelocity.x = -GHOST_SPEED * 0.50;
				m_direction = PacmanMoveDirection::LEFT;
				Look(m_direction);
			}
			else if (location.x < BLINKY_HOME_X) {
				nVelocity.x = GHOST_SPEED * 0.50;
				m_direction = PacmanMoveDirection::RIGHT;
				Look(m_direction);
			}
			else {
				if (m_direction != UP) {
					m_direction = PacmanMoveDirection::UP;
					Look(m_direction);
				}
				nVelocity.y = GHOST_SPEED * 0.45;
			}
		}
		Physics::SetVelocity(GetRealmID(), m_model_ID, nVelocity);
		
	}

	if (m_frame_timer.IsRunning()) {
		if (m_frame_timer.Update(deltaTime)) {
			m_frame_col += df;
			if (df < 0) {
				df = 1;
			}
			else {
				df = -1;
			}

			m_frame_timer.Start();
		}
	}

	if (m_blink_timer.IsRunning()) {
		if (m_blink_timer.Update(deltaTime)) {
			if (m_frame_col < 10) {
				m_frame_col += 2;
			}
			else {
				m_frame_col -= 2;
			}

			m_blink_timer.Start();
		}
	}
}

void Ghost::OnFinalUpdate()
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	const Vec2d worldCoords = m_map->GetWorldCoordinates(m_target_coords);
	double error = TILE_WIDTH / 5;
	
	if (m_is_home) {

		if (m_direction == UP) {

			if (m_home_timer.IsRunning()) {
				if (location.y > (GHOST_HOME_Y + MAX_HOME_Y_RANGE)) {
					Physics::SetPosition(GetRealmID(), m_model_ID, { location.x, GHOST_HOME_Y + MAX_HOME_Y_RANGE });
					m_direction = DOWN;
					Look(m_direction);
				}
			}
			else {
				if (location.y > (worldCoords.y - error)) {

					/* Ghost is completely out of home base. */
					Physics::SetPosition(GetRealmID(), m_model_ID, {location.x, worldCoords.y});

					int direction = Utils::RandInt(0, 1);
					if (direction == 0) {
						m_direction = LEFT;
						m_target_coords.x -= 1;
						Physics::SetVelocity(GetRealmID(), m_model_ID, { -m_speed, 0 });
					}
					else {
						m_direction = RIGHT;
						m_target_coords.x += 1;
						Physics::SetVelocity(GetRealmID(), m_model_ID, { m_speed, 0 });
					}

					m_is_home = false;
				}
			}
		}
		else if (m_direction == DOWN) {
			if (location.y < (GHOST_HOME_Y - MAX_HOME_Y_RANGE)) {
				Physics::SetPosition(GetRealmID(), m_model_ID, { location.x, GHOST_HOME_Y - MAX_HOME_Y_RANGE});
				m_direction = UP;
				Look(m_direction);
			}
		}
		else if (m_direction == LEFT) {
			if (location.x < BLINKY_HOME_X) {
				Physics::SetPosition(GetRealmID(), m_model_ID, { BLINKY_HOME_X, location.y });
				m_direction = UP;
				Look(m_direction);
			}
		}
		else if (m_direction == RIGHT) {
			if (location.x > BLINKY_HOME_X) {
				Physics::SetPosition(GetRealmID(), m_model_ID, { BLINKY_HOME_X, location.y });
				m_direction = UP;
				Look(m_direction);
			}
		}

		return;
	}


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

void Ghost::SetVulnerable(bool state)
{

	if (state) {

		m_frame_col = 8;
		m_frame_row = 0;
		df = 1;
		SetIsBlinking(false);

		if (m_is_vulnerable != state) {
			m_speed *= 0.40;
		}
	}
	else {
		SetIsBlinking(false);

		switch (m_name) {
		case BLINKY:
			m_frame_row = 0;
			break;
		case PINKY:
			m_frame_row = 1;
			break;
		case INKY:
			m_frame_row = 2;
			break;
		case CLYDE:
			m_frame_row = 3;
			break;
		default:
			m_frame_row = 0;
			break;
		}

		Look(m_direction);

		m_speed = GHOST_SPEED;
		if (m_name == BLINKY) {
			m_speed += 50;
		}
	}

	m_is_vulnerable = state;
}

void Ghost::SetIsBlinking(bool blink)
{
	m_is_blinking = blink;

	if (m_is_blinking) {
		m_blink_timer.Start();
	}
	else {
		m_blink_timer.End();
	}
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

void Ghost::Look(PacmanMoveDirection direction)
{
	if (m_is_vulnerable) {
		return;
	}

	switch (direction) {
	case LEFT:
		m_frame_col = 2;
		break;
	case RIGHT:
		m_frame_col = 0;
		break;
	case UP:
		m_frame_col = 4;
		break;
	case DOWN:
		m_frame_col = 6;
		break;
	}

	if (df < 0) {
		m_frame_col++;
	}
}

