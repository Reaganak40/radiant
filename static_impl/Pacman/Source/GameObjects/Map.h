#pragma once
#include "Game.h"

#define MAP_WIDTH 600
#define MAP_HEIGHT 664

class Map : public rdt::GameObject {
private:

	struct MapNode {
		int row, col;
		MapNode* pEdges[4];
	};

	std::array<std::array<MapNode*, NUM_TILES_X>, NUM_TILES_Y> m_graph;

	double m_realHeight;
public:
	Map();
	~Map();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;
	void OnMessage(rdt::Message msg) override final;

	bool IsInMap(int row, int col);
	rdt::Vec2i GetMapCoordinates(const rdt::Vec2d& worldCoords);
	rdt::Vec2d GetWorldCoordinates(const rdt::Vec2i& mapCoords);

	rdt::Vec2i GetLeftTeleport();
	rdt::Vec2i GetRightTeleport();

	void Djikstra(const rdt::Vec2i& start, const rdt::Vec2i& end, std::queue<PacmanMoveDirection>& path, PacmanMoveDirection currDirection = NOMOVE);

private:
	void CompileTileMap();
};
