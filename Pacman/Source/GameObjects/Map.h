#pragma once
#include "Game.h"

#define MAP_WIDTH 600
#define MAP_HEIGHT 664
#define MAP_LAYER 0

class Map : public rdt::GameObject {
private:
	std::vector<std::string> m_tilemap;
	double m_realHeight;
public:
	Map();
	~Map();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	bool IsInMap(int row, int col);
	rdt::Vec2i GetMapCoordinates(const rdt::Vec2d& worldCoords);
	rdt::Vec2d GetWorldCoordinates(const rdt::Vec2i& mapCoords);

private:
	void CompileTileMap();
};
