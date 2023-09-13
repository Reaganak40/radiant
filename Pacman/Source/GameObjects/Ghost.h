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

	int m_frame_row;
	int m_frame_col;
	Map* m_map;
public:
	Ghost(GhostName nName);
	~Ghost();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void AddMapPtr(Map* nMap);

};