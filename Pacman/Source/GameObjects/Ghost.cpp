#include "Ghost.h"

using namespace rdt;

Ghost::Ghost(GhostName nName)
	: m_frame_timer(0.35), m_blink_timer(0.20), m_path_finding_timer(0.6)
{
	spawnPos = Vec2d::Zero();
	m_target_coords = { 15, 10 };
	m_map = nullptr;
	m_pacman_ptr = nullptr;
	m_blinky_ptr = nullptr;
	m_is_vulnerable = false;
	m_paused = false;

	m_name = nName;
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
		m_speed = GHOST_SPEED;
	}
	else {
		m_is_home = true;
		m_speed = GHOST_SPEED;
	}

	m_frame_col = 0;
	df = 1;

	Look(m_direction);
	
	SetMovementMode(CHASE);
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

	Physics::SetObjectProperties(GetRealmID(), m_model_ID, DontResolve);
	Physics::AddPTag(GetRealmID(), m_model_ID, "pacman");
	Physics::SetHitBoxSize(GetRealmID(), m_model_ID, { 0.45, 0.45 });
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetFriction(GetRealmID(), m_model_ID, 0);

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
	if (m_paused) {
		return;
	}

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
		default:
			break;
		}

		if (m_direction != NOMOVE) {
			Look(m_direction);
		}

		Physics::SetVelocity(GetRealmID(), m_model_ID, nVelocity);
	
	} else if (m_is_home) {
		Vec2d nVelocity = Vec2d::Zero();

		if (m_is_eaten) {
			m_direction = DOWN;
			nVelocity.y = -m_speed;
		}
		else if (m_home_timer.IsRunning() && !m_home_timer.Update(deltaTime)) {

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
	else {
		if (Physics::GetVelocity(GetRealmID(), m_model_ID) == Vec2d::Zero()) {
			/* This should only happen to clyde on a new level. */
			Physics::SetVelocity(GetRealmID(), m_model_ID, { -GHOST_SPEED, 0 });
		}
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

	if (m_path_finding_timer.IsRunning()) {
		m_path_finding_timer.Update(deltaTime);
	}

	if (m_movement_timer.IsRunning()) {
		m_movement_timer.Update(deltaTime);
	}
}

void Ghost::OnFinalUpdate()
{
	if (m_paused) {
		return;
	}

	ResolveCollisions();
	FinalUpdatePosition();
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

void Ghost::AddBlinkyPtr(Ghost* blinky)
{
	if (m_name == INKY) {
		m_blinky_ptr = blinky;
	}
}

void Ghost::SetVulnerable(bool state)
{

	if (state) {

		if (m_is_home || m_is_eaten) {
			return;
		}

		m_frame_col = 8;
		m_frame_row = 0;
		df = 1;

		SetIsBlinking(false);

		/* If ghost not currently vulnerable */
		if (m_is_vulnerable != state) {
			m_speed *= 0.40;
		}
		m_is_vulnerable = state;

	}
	else {
		m_is_vulnerable = state;

		SetIsBlinking(false);

		ResetFrameRow();

		m_speed = GHOST_SPEED;
	}

}

void Ghost::SetIsBlinking(bool blink)
{
	if (blink && !m_is_vulnerable) {
		blink = false;
	}

	m_is_blinking = blink;

	if (m_is_blinking) {
		m_blink_timer.Start();
	}
	else {
		m_blink_timer.End();
	}
}

void Ghost::SetPause(bool pause)
{
	m_paused = pause;
}

void Ghost::SetMovementMode(MovementMode mode)
{
	m_movement_mode = mode;

	if (!m_direction_queue.empty()) {
		std::queue<PacmanMoveDirection> empty;
		std::swap(m_direction_queue, empty);
	}

	m_movement_timer.End();

	if (m_movement_mode == CHASE) {
		m_movement_timer.SetInterval(20);
		m_movement_timer.Start();

	}
	else if (m_movement_mode == SCATTER) {
		m_movement_timer.SetInterval(7);
		m_movement_timer.Start();
	}
}

void Ghost::SetPacmanPtr(Pacman* pacman)
{
	m_pacman_ptr = pacman;
}

void Ghost::SelectNewTarget()
{
	if (m_is_home) {
		return;
	}

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

	if (!m_movement_timer.IsRunning()) {
		if (m_movement_mode == CHASE) {
			SetMovementMode(SCATTER);
		}
		else if (m_movement_mode == SCATTER) {
			SetMovementMode(CHASE);
		}
	}

	switch (m_movement_mode) {
	case CHASE:
		if (!m_path_finding_timer.IsRunning()) {
			std::queue<PacmanMoveDirection> empty;
			std::swap(m_direction_queue, empty);
		}
		SelectNext();
		break;
	case SCATTER:
		SelectNext();
		break;
	case GOHOME:
		SelectNext();
		break;
	case FRIGHTENED:
		SelectRandom();
		break;
	}
}

void Ghost::SelectRandom()
{
	bool options[4];

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

void Ghost::SelectNext()
{
	/* Select next direction in the queue. */
	if (!m_direction_queue.empty()) {
		PacmanMoveDirection nDirection = m_direction_queue.front();

		switch (nDirection) {
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
		m_direction_queue.pop();
		return;
	}

	/* If nothing left in queue create new path. */
	if (m_movement_mode == CHASE) {
		CreateChasePath();
	}
	else if (m_movement_mode == SCATTER) {
		CreateScatterPath();
	}
	else if (m_movement_mode == GOHOME) {
		CreateHomePath();
	}

	SelectNewTarget();
}

void Ghost::CreateScatterPath()
{
	switch (m_name) {
	case BLINKY:

		if (m_target_coords == Vec2i(BLINKY_SCATTER_TARGET_X, BLINKY_SCATTER_TARGET_Y)) {
			/* Blinky scatter loop. */
			m_direction_queue.push(RIGHT);

			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);

			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);

			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			m_direction_queue.push(UP);

			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
		}
		else {
			CreateShortestPath({ BLINKY_SCATTER_TARGET_X, BLINKY_SCATTER_TARGET_Y });
		}
		break;
	case PINKY:
		if (m_target_coords == Vec2i(PINKY_SCATTER_TARGET_X, PINKY_SCATTER_TARGET_Y)) {
			/* Pinky scatter loop. */
			m_direction_queue.push(LEFT);

			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);

			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);

			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			m_direction_queue.push(UP);

			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
		}
		else {
			CreateShortestPath({ PINKY_SCATTER_TARGET_X, PINKY_SCATTER_TARGET_Y });
		}
		break;
	case INKY:
		if (m_target_coords == Vec2i(INKY_SCATTER_TARGET_X, INKY_SCATTER_TARGET_Y)) {
			/* Inky scatter loop. */
			m_direction_queue.push(RIGHT);

			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);

			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			m_direction_queue.push(UP);

			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);

			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);

			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);

			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);

			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
		}
		else {
			CreateShortestPath({ INKY_SCATTER_TARGET_X, INKY_SCATTER_TARGET_Y });
		}
		break;
	case CLYDE:
		if (m_target_coords == Vec2i(CLYDE_SCATTER_TARGET_X, CLYDE_SCATTER_TARGET_Y)) {
			/* Clyde scatter loop. */
			m_direction_queue.push(LEFT);

			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			m_direction_queue.push(UP);

			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);

			m_direction_queue.push(UP);
			m_direction_queue.push(UP);
			m_direction_queue.push(UP);

			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);

			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);

			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);
			m_direction_queue.push(RIGHT);

			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);
			m_direction_queue.push(DOWN);

			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
			m_direction_queue.push(LEFT);
		}
		else {
			CreateShortestPath({ CLYDE_SCATTER_TARGET_X, CLYDE_SCATTER_TARGET_Y });
		}
		break;
	}
}

void Ghost::CreateChasePath()
{
	switch (m_name) {
	case BLINKY:
		CreateShortestPath(m_pacman_ptr->GetMapCoordinates());
		break;
	case PINKY:
	{
		Vec2i target = m_pacman_ptr->GetMapCoordinates();

		switch (m_pacman_ptr->GetDirection()) {
		case UP:
			target.y -= 4;
			if (target.y < 0) {
				target.y += 8;
			}
			break;
		case LEFT:
			target.x -= 4;
			if (target.x < 3) {
				target.x += 8;
			}
			break;
		case RIGHT:
			target.x += 4;
			if (target.x > 28) {
				target.x -= 8;
			}
			break;
		case DOWN:
			target.y += 4;
			if (target.y > 28) {
				target.y -= 8;
			}
			break;
		}

		while (!m_map->IsInMap(target.y, target.x)) {
			if (target.x < 28) {
				target.x += 1;
			}
			else {
				target.y += 1;
			}
		}

		CreateShortestPath(target);
	}
		break;
	case INKY:
	{
		Vec2i target = m_pacman_ptr->GetMapCoordinates();

		switch (m_pacman_ptr->GetDirection()) {
		case UP:
			target.y -= 2;
			if (target.y < 0) {
				target.y += 4;
			}
			break;
		case LEFT:
			target.x -= 2;
			if (target.x < 3) {
				target.x += 4;
			}
			break;
		case RIGHT:
			target.x += 2;
			if (target.x > 28) {
				target.x -= 4;
			}
			break;
		case DOWN:
			target.y += 2;
			if (target.y > 28) {
				target.y -= 4;
			}
			break;
		}
		Vec2i blinkyPos = m_blinky_ptr->GetMapCoordinates();
		target -= blinkyPos;
		target *= 2;
		target = blinkyPos + target;

		while (!m_map->IsInMap(target.y, target.x)) {
			if (target.x > 28) {
				target.x--;
			}
			else if (target.x < 3) {
				target.x++;
			}
			else if (target.y < 0) {
				target.y++;
			}
			else if (target.y > 28) {
				target.y--;
			}
			else if (target.x < 28) {
				target.x += 1;
			}
			else {
				target.y += 1;
			}
		}
		CreateShortestPath(target);
	}
		break;
	case CLYDE:
	{
		Vec2i target = m_pacman_ptr->GetMapCoordinates();
		Vec2i currPos = GetMapCoordinates();

		if (Vabs(target - currPos).Magnitude() > 8) {
			CreateShortestPath(m_pacman_ptr->GetMapCoordinates());
		}
		else {
			CreateShortestPath({ CLYDE_SCATTER_TARGET_X, CLYDE_SCATTER_TARGET_Y });
		}
	}
		break;
	}

	m_path_finding_timer.Start();
}

void Ghost::CreateHomePath()
{
	if (GetMapCoordinates() == Vec2i(15, 10)) {
		m_direction = NOMOVE;
		m_is_home = true;
	}
	else {
		CreateShortestPath({15, 10});
	}
}

void Ghost::CreateShortestPath(rdt::Vec2i target)
{
	/* Use Djikstra shortest path to create the direction queue. */
	m_map->Djikstra(m_target_coords, target, m_direction_queue, m_direction);

	if (m_direction_queue.empty()) {
		m_movement_mode = FRIGHTENED;
	}
}

rdt::Vec2i Ghost::GetMapCoordinates()
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	return m_map->GetMapCoordinates(location);
}

void Ghost::Look(PacmanMoveDirection direction)
{
	if (m_is_vulnerable) {
		return;
	}

	switch (direction) {
	case LEFT:
		if (m_is_eaten) {
			m_frame_col = 9;
		}
		else {
			m_frame_col = 2;
		}
		break;
	case RIGHT:
		if (m_is_eaten) {
			m_frame_col = 8;
		}
		else {
			m_frame_col = 0;
		}
		break;
	case UP:
		if (m_is_eaten) {
			m_frame_col = 10;
		}
		else {
			m_frame_col = 4;
		}
		break;
	case DOWN:
		if (m_is_eaten) {
			m_frame_col = 11;
		}
		else {
			m_frame_col = 6;
		}
		break;
	}

	if (!m_is_eaten && df < 0) {
		m_frame_col++;
	}
}

void Ghost::ResolveCollisions()
{
	UniqueID pacmanModelID = m_pacman_ptr->GetModelID();

	if (Physics::IsCollided(GetRealmID(), m_model_ID, pacmanModelID)) {
		
		switch (m_name) {
		case BLINKY:
			printf("blinky\n");
			break;
		case PINKY:
			printf("pinky\n");
			break;
		case INKY:
			printf("inky\n");
			break;
		case CLYDE:
			printf("clyde\n");
			break;
		default:
			break;
		}
		if (m_is_vulnerable) {
			OnEaten();
		}
	}
}

void Ghost::FinalUpdatePosition()
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	const Vec2d worldCoords = m_map->GetWorldCoordinates(m_target_coords);
	double error = TILE_WIDTH / 6;

	if (m_is_home) {
		
		if (m_is_eaten) {
			Physics::SetPosition(GetRealmID(), m_model_ID, { PINKY_HOME_X - 3, location.y });

			if (location.y < GHOST_HOME_Y) {
				OnRevived();
			}

			return;
		}

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
					Physics::SetPosition(GetRealmID(), m_model_ID, { location.x, worldCoords.y });

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
				Physics::SetPosition(GetRealmID(), m_model_ID, { location.x, GHOST_HOME_Y - MAX_HOME_Y_RANGE });
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

void Ghost::OnEaten()
{
	m_is_eaten = true;
	SetVulnerable(false);
	m_frame_row = 1;
	m_frame_timer.End();
	
	SetMovementMode(GOHOME);
	CreateShortestPath({ 15, 10 });
}

void Ghost::OnRevived()
{
	m_is_eaten = false;
	ResetFrameRow();
	SetMovementMode(CHASE);
}

void Ghost::ResetFrameRow()
{
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
}

