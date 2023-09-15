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
	FRIGHTENED
};

class Ghost : public rdt::GameObject {
private:
	GhostName m_name;
	rdt::Vec2d spawnPos;
	double m_speed;

	rdt::Timer m_frame_timer;
	int df;
	int m_frame_row;
	int m_frame_col;

	Map* m_map;
	rdt::Vec2i m_target_coords;
	PacmanMoveDirection m_direction;

	MovementMode m_movement_mode;
	std::queue<PacmanMoveDirection> m_direction_queue;
	rdt::Timer m_path_finding_timer;

	rdt::Timer m_home_timer;
	bool m_is_home;

	bool m_is_vulnerable;
	bool m_is_blinking;
	rdt::Timer m_blink_timer;

	bool m_paused;

	Pacman* m_pacman_ptr;
	Ghost* m_blinky_ptr;

public:
	Ghost(GhostName nName);
	~Ghost();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void AddMapPtr(Map* nMap);
	void AddBlinkyPtr(Ghost* blinky);

	void SetVulnerable(bool state);
	void SetIsBlinking(bool blink);
	void SetPause(bool pause);

	void SetMovementMode(MovementMode mode);

	void SetPacmanPtr(Pacman* pacman);

private:
	void SelectNewTarget();
	void SelectRandom();
	void SelectNext();

	void CreateScatterPath();
	void CreateChasePath();
	void CreateShortestPath(rdt::Vec2i target);

	rdt::Vec2i GetMapCoordinates();


	void Look(PacmanMoveDirection direction);

	void ResolveCollisions();
	void FinalUpdatePosition();
};