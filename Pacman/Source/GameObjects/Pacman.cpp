#include "Pacman.h"

using namespace rdt;
Pacman::Pacman(double xPos, double yPos)
	: m_texture_timer(Timer(0.02)), m_death_timer(0.09)
{
	GState.SetStateCount(PGS_MaxState);
	spawnPos.x = xPos;
	spawnPos.y = yPos;

	left_cond = std::vector<InputState>{ A_KEY_PRESS, A_KEY_DOWN, LEFT_KEY_DOWN, LEFT_KEY_PRESS };
	right_cond = std::vector<InputState>{ D_KEY_PRESS, D_KEY_DOWN, RIGHT_KEY_DOWN, RIGHT_KEY_PRESS };
	up_cond = std::vector<InputState>{ W_KEY_PRESS, W_KEY_DOWN, UP_KEY_DOWN, UP_KEY_PRESS };
	down_cond = std::vector<InputState>{ S_KEY_PRESS, S_KEY_DOWN, DOWN_KEY_DOWN, DOWN_KEY_PRESS };

	m_map = nullptr;
	current_frame = 2;
	df = -1;
	
	RegisterToMessageBus("pacman");
}

Pacman::~Pacman()
{
}

void Pacman::OnBind()
{
	SendDirectMessage("map", MT_RequestGameObjectPtr, nullptr);
	AddObjectToWorld(std::make_shared<Rect>(spawnPos, PACMAN_SPRITE_WIDTH, PACMAN_SPRITE_WIDTH));

	Physics::SetObjectProperties(GetRealmID(), m_model_ID, DontResolve);
	Physics::AddPTag(GetRealmID(), m_model_ID, "pacman");
	Physics::SetHitBoxSize(GetRealmID(), m_model_ID, {0.35, 0.35});
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetFriction(GetRealmID(), m_model_ID, 0);

	Respawn();
}

void Pacman::OnRelease()
{
}

void Pacman::OnProcessInput(const float deltaTime)
{
	if (GState.CheckState(PGS_Paused)) {
		return;
	}
	
	UpdateVelocityAndDirection();
	UpdateTextureFrame(deltaTime);
}

void Pacman::OnFinalUpdate()
{
	if (GState.CheckState(PGS_Paused)) {
		return;
	}

	ReAlignToMap();
}

void Pacman::OnRender()
{
	using namespace rdt;

	if (GState.CheckState(PGS_IsGameOver) || m_frame_col == 10) {
		return;
	}

	Renderer::Begin(PACMAN_LAYER);

	if (GState.CheckState(PGS_IsHit)) {
		Renderer::SetPolygonTexture("death", m_frame_col, 0);
	}
	else if (current_frame == 2)  {
		Renderer::SetPolygonTexture("pacman", 2, 0);
	}
	else {
		Renderer::SetPolygonTexture("pacman", m_frame_col, m_frame_row);

	}

	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}

void Pacman::OnMessage(rdt::Message msg)
{
	switch (msg.type) {
	case MT_RequestGameObjectPtr:
		SendMessage(msg.from, MT_SendGameObjectPtr, new GameObjectPtrData(this));
		break;
	case PMT_PacmanHit:
		OnHit();
		break;
	case MT_SendGameObjectPtr:
		AddGameObjectPtr(msg.from, (GameObjectPtrData*)msg.data);
		break;
	case PMT_StartDeathAnimation:
		BeginDeathAnimation();
		break;
	case PMT_PauseGame:
		SetPause(true);
		break;
	case PMT_ResumeGame:
		SetPause(false);
		break;
	case PMT_Respawn:
		Respawn();
		break;
	case PMT_GameOver:
		GState.SetState(PGS_IsGameOver, true);
		break;
	}
}

void Pacman::Respawn()
{
	Physics::SetPosition(GetRealmID(), m_model_ID, { PACMAN_SPAWN_X, m_map->GetWorldCoordinates({12, 22}).y});
	m_frame_col = 1;
	m_frame_row = 0;
	current_frame = 2;
	df = -1;
	m_texture_timer.Start();

	GState.SetState(PGS_Spawned, true);
	GState.SetState(PGS_InRespawn, false);
}

rdt::Vec2i Pacman::GetMapCoordinates()
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	return m_map->GetMapCoordinates(location);
}

rdt::Vec2d Pacman::GetWorldCoordinates()
{
	return Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
}

PacmanMoveDirection Pacman::GetDirection()
{
	return m_direction;
}

void Pacman::SetPause(bool pause)
{
	GState.SetState(PGS_Paused, pause);
}

void Pacman::BeginDeathAnimation()
{
	GState.SetState(PGS_InDeathAnimation, true);
	GState.SetState(PGS_InRespawn, true);
	m_death_timer.Start();
}

bool Pacman::InRespawn()
{
	return GState.CheckState(PGS_InRespawn);
}

bool Pacman::IsHit()
{
	return GState.CheckState(PGS_IsHit);
}

void Pacman::AddGameObjectPtr(rdt::MessageID from, rdt::GameObjectPtrData* data)
{
	if (MessageBus::GetAlias(from) == "map") {
		m_map = (Map*)data->ptr;
	}
}

void Pacman::UpdateVelocityAndDirection()
{
	Vec2d nVelocity = Vec2d::Zero();

	if (GState.CheckState(PGS_InDeathAnimation)) {
		return;
	}

	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	Vec2i mapCoords = m_map->GetMapCoordinates(location);
	Vec2d centeredCoords = m_map->GetWorldCoordinates(mapCoords);

	if (GState.CheckState(PGS_Spawned)) {
		m_direction = PacmanMoveDirection::RIGHT;
		m_target_coords = mapCoords;
		GState.SetState(PGS_Spawned, false);
	}

	bool options[4] = { false };
	bool keys[4] = { true, true, true, true };

	/* Check legal moves */
	/* Check can move right. */
	if ((m_direction == RIGHT || m_direction == LEFT) && (location.x < centeredCoords.x || m_map->IsInMap(mapCoords.y, mapCoords.x + 1))) {
		options[RIGHT] = true;
	}
	else if (location.y == centeredCoords.y && m_map->IsInMap(mapCoords.y, mapCoords.x + 1)) {
		options[RIGHT] = true;
	}

	/* Check can move left. */
	if ((m_direction == RIGHT || m_direction == LEFT) && (location.x > centeredCoords.x || m_map->IsInMap(mapCoords.y, mapCoords.x - 1))) {
		options[LEFT] = true;
	}
	else if (location.y == centeredCoords.y && m_map->IsInMap(mapCoords.y, mapCoords.x - 1)) {
		options[LEFT] = true;
	}

	/* Check can move up. */
	if ((m_direction == UP || m_direction == DOWN) && (location.y < centeredCoords.y || m_map->IsInMap(mapCoords.y - 1, mapCoords.x))) {
		options[UP] = true;
	}
	else if (location.x == centeredCoords.x && m_map->IsInMap(mapCoords.y - 1, mapCoords.x)) {
		options[UP] = true;
	}

	/* Check can move down. */
	if ((m_direction == UP || m_direction == DOWN) && (location.y > centeredCoords.y || m_map->IsInMap(mapCoords.y + 1, mapCoords.x))) {
		options[DOWN] = true;
	}
	else if (location.x == centeredCoords.x && m_map->IsInMap(mapCoords.y + 1, mapCoords.x)) {
		options[DOWN] = true;
	}

	/* Check if player wants to go left or right and the move is legal. */
	if (Input::CheckKeyboardState(right_cond)) {
		keys[LEFT] = false;
	}
	else if (Input::CheckKeyboardState(left_cond)) {
		keys[RIGHT] = false;
	}
	else {
		keys[LEFT] = false;
		keys[RIGHT] = false;
	}

	/* Check if player wants to go up or down and the move is legal. */
	if (Input::CheckKeyboardState(up_cond)) {
		keys[DOWN] = false;
	}
	else if (Input::CheckKeyboardState(down_cond)) {
		keys[UP] = false;
	}
	else {
		keys[UP] = false;
		keys[DOWN] = false;
	}

	/* If the player has multiple keys down and both options are legal directions. */
	bool resolved = false;
	if ((keys[LEFT] || keys[RIGHT]) && (keys[UP] || keys[DOWN])) {

		if (m_direction == LEFT || m_direction == DOWN) {
			if (options[UP] && keys[UP]) {
				m_direction = PacmanMoveDirection::UP;
				resolved = true;
			}
			else if (options[DOWN] && keys[DOWN]) {
				m_direction = PacmanMoveDirection::DOWN;
				resolved = true;
			}
		}
		else {
			if (options[LEFT] && keys[LEFT]) {
				m_direction = PacmanMoveDirection::LEFT;
				resolved = true;
			}
			else if(options[RIGHT] && keys[RIGHT]){
				m_direction = PacmanMoveDirection::RIGHT;
				resolved = true;
			}
		}
	}
	/* The user has keyDown with one or no legal directions(s). */
	else if (keys[LEFT] || keys[RIGHT] || keys[UP] || keys[DOWN]) {
		
		for (int i = 0; i < 4; i++) {
			if (keys[i] && options[i]) {
				switch (i) {
				case UP:
					m_direction = PacmanMoveDirection::UP;
					break;
				case DOWN:
					m_direction = PacmanMoveDirection::DOWN;
					break;
				case LEFT:
					m_direction = PacmanMoveDirection::LEFT;
					break;
				case RIGHT:
					m_direction = PacmanMoveDirection::RIGHT;
					break;
				}
				resolved = true;
				break;
			}
		}
	}

	/* If the direction is still not resolved, use the existing direction unless hit a wall. */
	if (!resolved) {
		if (!options[m_direction]) {
			m_direction = PacmanMoveDirection::NOMOVE;
		}
	}

	switch (m_direction) {
	case UP:
		if (location.y >= centeredCoords.y) {
			m_target_coords = { mapCoords.x, mapCoords.y - 1 };
		}
		nVelocity.y = PACMAN_SPEED;
		break;
	case DOWN:
		if (location.y <= centeredCoords.y) {
			m_target_coords = { mapCoords.x, mapCoords.y + 1 };
		}
		nVelocity.y = -PACMAN_SPEED;
		break;
	case LEFT:
		if (location.x <= centeredCoords.x) {
			m_target_coords = { mapCoords.x - 1, mapCoords.y };
		}
		nVelocity.x = -PACMAN_SPEED;
		break;
	case RIGHT:
		if (location.x >= centeredCoords.x) {
			m_target_coords = { mapCoords.x + 1, mapCoords.y };
		}
		nVelocity.x = PACMAN_SPEED;
		break;
	default:
		break;
	}

	Physics::SetVelocity(GetRealmID(), m_model_ID, nVelocity);
}

void Pacman::UpdateTextureFrame(const float deltaTime)
{
	if (m_texture_timer.IsRunning()) {
		if (m_texture_timer.Update(deltaTime)) {
			current_frame += df;

			if (current_frame == 0) {
				df = 1;
			}
			else if (current_frame == 2) {
				df = -1;
			}

			m_frame_col = current_frame;
			m_texture_timer.Start();
		}
	}
	else if (m_death_timer.IsRunning()) {
		if (m_death_timer.Update(deltaTime)) {
			m_frame_col++;

			if (m_frame_col == 10) {
				GState.SetState(PGS_InDeathAnimation, false);
				GState.SetState(PGS_IsHit, false);
				SendMessage("level", PMT_EndDeathAnimation);
			}
			else {
				m_death_timer.Start();
			}

		}
	}

	if (GState.CheckState(PGS_IsHit)) {
		return;
	}

	if (m_direction == NOMOVE) {
		current_frame = 1;
		m_frame_col = 1;
	}
	else {
		switch (m_direction) {
		case UP:
			m_frame_row = 2;
			break;
		case DOWN:
			m_frame_row = 3;
			break;
		case LEFT:
			m_frame_row = 1;
			break;
		case RIGHT:
			m_frame_row = 0;
			break;
		default:
			break;
		}
	}
}

void Pacman::ReAlignToMap()
{
	const Vec2d location = Physics::GetPolygon(GetRealmID(), m_model_ID).GetOrigin();
	const Vec2d centeredCoords = m_map->GetWorldCoordinates(m_target_coords);

	/* Player is off screen left or right and needs to teleport to other side of map*/
	if (m_map->GetLeftTeleport() == m_target_coords && (location.x < centeredCoords.x)) {
		m_target_coords = m_map->GetRightTeleport();
		Physics::SetPosition(GetRealmID(), m_model_ID, m_map->GetWorldCoordinates(m_target_coords));
		m_target_coords.x -= 1;
	}
	else if (m_map->GetRightTeleport() == m_target_coords && (location.x > centeredCoords.x)) {
		m_target_coords = m_map->GetLeftTeleport();
		Physics::SetPosition(GetRealmID(), m_model_ID, m_map->GetWorldCoordinates(m_target_coords));
		m_target_coords.x += 1;
	}
	else {

		switch (m_direction) {
		case UP:
			if (location.y > centeredCoords.y) {
				Physics::SetPosition(GetRealmID(), m_model_ID, centeredCoords);
			}
			break;
		case DOWN:
			if (location.y < centeredCoords.y) {
				Physics::SetPosition(GetRealmID(), m_model_ID, centeredCoords);
			}
			break;
		case LEFT:
			if (location.x < centeredCoords.x) {
				Physics::SetPosition(GetRealmID(), m_model_ID, centeredCoords);
			}
			break;
		case RIGHT:
			if (location.x > centeredCoords.x) {
				Physics::SetPosition(GetRealmID(), m_model_ID, centeredCoords);
			}
			break;
		default:
			break;
		}
	}
}

void Pacman::OnHit()
{
	GState.SetState(PGS_IsHit, true);
	m_frame_col = 0;
	m_texture_timer.End();
	SendDirectMessage("level", PMT_PacmanHit);
}

