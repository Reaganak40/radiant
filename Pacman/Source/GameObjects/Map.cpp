#include "Map.h"

Map::Map()
{
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
