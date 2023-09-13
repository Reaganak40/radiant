#pragma once

#include "Game.h"
#include "Map.h"

enum GhostName {
	BLINKY,
	INKY,
	PINKY,
	CLYDE
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

	rdt::Timer m_home_timer;
	bool m_is_home;

	bool m_is_vulnerable;
	bool m_is_blinking;
	rdt::Timer m_blink_timer;

public:
	Ghost(GhostName nName);
	~Ghost();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void AddMapPtr(Map* nMap);

	void SetVulnerable(bool state);
	void SetIsBlinking(bool blink);

private:
	void SelectNewTarget();
	void Look(PacmanMoveDirection direction);
};