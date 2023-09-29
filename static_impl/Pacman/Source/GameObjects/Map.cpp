#include "Map.h"
#include <iostream>
#include <queue>

using namespace rdt;

Map::Map()
{
	CompileTileMap();
	m_realHeight = TILE_WIDTH * NUM_TILES_Y;
	RegisterToMessageBus("map");
}

Map::~Map()
{
	for (int row = 0; row < NUM_TILES_Y; row++) {
		for (int col = 0; col < NUM_TILES_X; col++) {
			if (m_graph[row][col] != nullptr) {
				delete m_graph[row][col];
			}
		}
	}
}

void Map::OnBind()
{
	using namespace rdt;

	Vec2d spawnPos = Vec2d(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 10);
	m_model_ID = Physics::CreateObject(GetRealmID(), GetMessageID(), std::make_shared<Rect>(spawnPos, MAP_WIDTH, MAP_HEIGHT));
	
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, NoCollision);
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

void Map::OnMessage(rdt::Message msg)
{
	switch (msg.type) {
	case MT_RequestGameObjectPtr:
		SendDirectMessage(msg.from, MT_SendGameObjectPtr, new GameObjectPtrData(this));
		break;
	}
}

bool Map::IsInMap(int row, int col)
{
	if (row < 0 || row >= NUM_TILES_Y) {
		return false;
	}

	if (col < 0 || col >= NUM_TILES_X) {
		return false;
	}
	return m_graph[row][col] != nullptr;
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

rdt::Vec2i Map::GetLeftTeleport()
{
	return { 0, 13 };
}

rdt::Vec2i Map::GetRightTeleport()
{
	return { 31, 13 };
}

void Map::Djikstra(const rdt::Vec2i& start, const rdt::Vec2i& end, std::queue<PacmanMoveDirection>& path, PacmanMoveDirection currDirection)
{
	if (start.x == end.x && start.y == end.y) {
		return;
	}

	if (currDirection != NOMOVE) {
		switch (currDirection) {
		case UP:
			currDirection = DOWN;
			break;
		case DOWN:
			currDirection = UP;
			break;
		case LEFT:
			currDirection = RIGHT;
			break;
		case RIGHT:
			currDirection = LEFT;
			break;
		}
	}

	/* Distance matrix to backtrack and find shortest path. */
	std::array<std::array<int, NUM_TILES_X>, NUM_TILES_Y> distances;
	std::array<std::array<PacmanMoveDirection, NUM_TILES_X>, NUM_TILES_Y> directions;
	for (int row = 0; row < NUM_TILES_Y; row++) {
		distances[row].fill(INT_MAX);
		directions[row].fill(NOMOVE);
	}

	/*
		Set Node <int, int, int> ==> <distance, row, col>
	*/
	std::set<std::tuple<int, int, int>> pq;

	distances[start.y][start.x] = 0;
	pq.insert({0, start.y, start.x});

	while (pq.size()) {

		/* Pop smallest element */
		auto current = *pq.begin();
		pq.erase(pq.begin());

		int uDistance = std::get<0>(current);
		int uRow = std::get<1>(current);
		int uCol = std::get<2>(current);

		/* Found target node. */
		if (uRow == end.y && uCol == end.x) {
			break;
		}

		/* Traverse all neighbors */
		for(int i = 0; i < 4; i++) {
			if (i == currDirection && uRow == start.y && uCol == start.x) {
				continue;
			}

			MapNode* edge = m_graph[uRow][uCol]->pEdges[i];
			if (edge == nullptr) {
				continue;
			}

			int vRow = edge->row;
			int vCol = edge->col;
			int vDistance = distances[vRow][vCol];
			if (uDistance + 1 < vDistance) {

				if (vDistance != INT_MAX) {
					pq.erase(pq.find(std::make_tuple(vDistance, vRow, vCol)));
				}

				distances[vRow][vCol] = uDistance + 1;
				pq.insert(std::make_tuple(distances[vRow][vCol], vRow, vCol));

				switch (i) {
				case UP:
					directions[vRow][vCol] = DOWN;
					break;
				case DOWN:
					directions[vRow][vCol] = UP;
					break;
				case LEFT:
					directions[vRow][vCol] = RIGHT;
					break;
				case RIGHT:
					directions[vRow][vCol] = LEFT;
					break;
				}
			}
		}
	}

	/* If the target node was never found. */
	if (directions[end.y][end.x] == NOMOVE) {
		return;
	}

	std::vector<PacmanMoveDirection> retrace;

	int currRow = end.y;
	int currCol = end.x;

	while (currRow != start.y || currCol != start.x) {
		MapNode* pNext = nullptr;
		switch (directions[currRow][currCol]) {
		case UP:
			retrace.push_back(DOWN);
			pNext = m_graph[currRow][currCol]->pEdges[UP];
			break;
		case DOWN:
			retrace.push_back(UP);
			pNext = m_graph[currRow][currCol]->pEdges[DOWN];
			break;
		case LEFT:
			retrace.push_back(RIGHT);
			pNext = m_graph[currRow][currCol]->pEdges[LEFT];
			break;
		case RIGHT:
			retrace.push_back(LEFT);
			pNext = m_graph[currRow][currCol]->pEdges[RIGHT];
			break;
		}

		if (pNext != nullptr) {
			currRow = pNext->row;
			currCol = pNext->col;
		}
	}

	while (!retrace.empty()) {
		path.push(retrace.back());
		retrace.pop_back();
	}
}

void Map::CompileTileMap()
{
	using namespace rdt;
	std::vector<std::string> tilemap;

	std::string res;
	Utils::ReadTextFile("Resources/TileMap.txt", res);
	
	std::stringstream ss(res);
	std::string token;
	while (std::getline(ss, token, '\n')) {
		while (token.length() < NUM_TILES_X) {
			token += " ";
		}
		tilemap.push_back(token);
	}

	for (int row = 0; row < NUM_TILES_Y; row++) {
		for (int col = 0; col < NUM_TILES_X; col++) {
			if (tilemap[row][col] != '#') {
				m_graph[row][col] = nullptr;
				continue;
			}

			m_graph[row][col] = new MapNode;
			m_graph[row][col]->row = row;
			m_graph[row][col]->col = col;

			if (row == 0) {
				m_graph[row][col]->pEdges[UP] = nullptr;
			}
			else {
				m_graph[row][col]->pEdges[UP] = m_graph[row - 1][col];

				if (m_graph[row - 1][col] != nullptr) {
					m_graph[row - 1][col]->pEdges[DOWN] = m_graph[row][col];
				}
			}

			if (col == 0) {
				m_graph[row][col]->pEdges[LEFT] = nullptr;
			}
			else {
				m_graph[row][col]->pEdges[LEFT] = m_graph[row][col-1];

				if (m_graph[row][col - 1] != nullptr) {
					m_graph[row][col-1]->pEdges[RIGHT] = m_graph[row][col];
				}
			}

			m_graph[row][col]->pEdges[RIGHT] = nullptr;
			m_graph[row][col]->pEdges[DOWN] = nullptr;
		}
	}

	m_graph[13][0]->pEdges[LEFT] = m_graph[13][31];
	m_graph[13][31]->pEdges[RIGHT] = m_graph[13][0];
}
