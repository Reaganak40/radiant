#pragma once

#include "Game.h"
#include "Map.h"
#include "Pacman.h"

#define BLINKY_SCATTER_TARGET_X 27
#define BLINKY_SCATTER_TARGET_Y 0
#define PINKY_SCATTER_TARGET_X 4
#define PINKY_SCATTER_TARGET_Y 0
#define INKY_SCATTER_TARGET_X 27
#define INKY_SCATTER_TARGET_Y 28
#define CLYDE_SCATTER_TARGET_X 4
#define CLYDE_SCATTER_TARGET_Y 28



enum GhostName {
	BLINKY,
	INKY,
	PINKY,
	CLYDE
};

enum MovementMode {
	CHASE,
	SCATTER,
	FRIGHTENED,
	GOHOME,
};

enum GhostGameState {
	GSS_Paused,
	GSS_IsHome,
	GSS_ShouldLeave,
	GSS_IsVulnerable,
	GSS_IsBlinking,
	GSS_IsEaten,
	GSS_MaxState,
};
class Ghost : public rdt::GameObject {
private:
	GhostName m_name;
	std::string m_nameStr;
	rdt::Vec2d spawnPos;
	double m_speed;

	Map* m_map_ptr;
	Pacman* m_pacman_ptr;
	Ghost* m_blinky_ptr;

	rdt::Timer m_frame_timer;
	rdt::Timer m_path_finding_timer;
	rdt::Timer m_movement_timer;
	rdt::Timer m_blink_timer;
	
	int df;
	int m_frame_row;
	int m_frame_col;

	rdt::Vec2i m_target_coords;
	PacmanMoveDirection m_direction;
	MovementMode m_movement_mode;
	std::queue<PacmanMoveDirection> m_direction_queue;

public:
	Ghost(GhostName nName);
	~Ghost();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;

	void SetVulnerable(bool state);
	void SetIsBlinking(bool blink);

	void SetMovementMode(MovementMode mode);

private:
	void Respawn();
	void AddGameObjectPtr(rdt::MessageID from, rdt::GameObjectPtrData* data);

	void SetPause(bool pause);
	void SelectNewTarget();
	void SelectRandom();
	void SelectNext();

	void CreateScatterPath();
	void CreateChasePath();
	void CreateHomePath();
	void CreateShortestPath(rdt::Vec2i target);

	rdt::Vec2i GetMapCoordinates();


	void Look(PacmanMoveDirection direction);

	void ResolveCollision(rdt::CollisionData* data);
	void FinalUpdatePosition();
	void OnEaten();
	void OnRevived();

	void ResetFrameRow();
};