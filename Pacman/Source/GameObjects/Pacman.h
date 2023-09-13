#pragma once
#include "Game.h"
#include "Map.h"

class Pacman : public rdt::GameObject {
private:
	rdt::Vec2d spawnPos;

	std::vector<rdt::InputState> left_cond;
	std::vector<rdt::InputState> right_cond;
	std::vector<rdt::InputState> up_cond;
	std::vector<rdt::InputState> down_cond;

	bool m_lastMove[4];
	bool m_onBorder[4];
	rdt::Vec2i m_lastMapCoords;

	Map* m_map;
	int current_frame;
	int df;

	rdt::Timer m_timer;

public:
	Pacman(double xPos, double yPos);
	~Pacman();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void AddMapPtr(Map* map);

	void Respawn();

	rdt::Vec2i GetMapCoordinates();

private:
	void UpdateBorderCheck(const rdt::Vec2i& mapCoords);
};