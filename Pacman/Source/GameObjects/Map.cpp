#include "Map.h"
#include <iostream>

Map::Map()
{
	CompileTileMap();
	m_realHeight = TILE_WIDTH * NUM_TILES_Y;
}

Map::~Map()
{
}

void Map::OnBind()
{
	using namespace rdt;

	Vec2d spawnPos = Vec2d(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 10);
	m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, MAP_WIDTH, MAP_HEIGHT));
	
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppRigid);
	Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());

	

}

void Map::OnRelease()
{
}

void Map::OnProcessInput(const float deltaTIme)
{
}

void Map::OnFinalUpdate()
{
}

void Map::OnRender()
{
	using namespace rdt;

	Renderer::Begin(MAP_LAYER);
	Renderer::SetPolygonTexture("map");
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}

bool Map::IsInMap(int row, int col)
{
	if (row < 0 || row >= NUM_TILES_Y) {
		return false;
	}

	if (col < 0 || col >= NUM_TILES_X) {
		return false;
	}
	return m_tilemap[row][col] == '#';
}

rdt::Vec2i Map::GetMapCoordinates(const rdt::Vec2d& worldCoords)
{
	return rdt::Vec2i(
		(int)((worldCoords.x - TILE_OFFSET_X) / TILE_WIDTH),
		(int)((m_realHeight + TILE_OFFSET_Y - worldCoords.y) / TILE_WIDTH));
}

rdt::Vec2d Map::GetWorldCoordinates(const rdt::Vec2i& mapCoords)
{
	return rdt::Vec2d(mapCoords.x * TILE_WIDTH + TILE_OFFSET_X, m_realHeight+TILE_OFFSET_Y-(mapCoords.y * TILE_WIDTH));
}

void Map::CompileTileMap()
{
	using namespace rdt;

	std::string res;
	Utils::ReadTextFile("Resources/TileMap.txt", res);
	
	std::stringstream ss(res);
	std::string token;
	while (std::getline(ss, token, '\n')) {
		while (token.length() < NUM_TILES_X) {
			token += " ";
		}
		m_tilemap.push_back(token);
	}
}
