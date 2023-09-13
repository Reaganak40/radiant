#include "Ghost.h"

using namespace rdt;

Ghost::Ghost(GhostName nName)
{
	m_name = nName;
	spawnPos = Vec2d::Zero();
	m_map = nullptr;

	switch (nName) {
	case BLINKY:
		m_frame_row = 0;
		break;
	case PINKY:
		m_frame_row = 1;
		break;
	case INKY:
		m_frame_row = 2;
		break;
	case CLYDE:
		m_frame_row = 3;
		break;
	default:
		m_frame_row = 0;
		break;
	}
	m_frame_col = 0;
}

Ghost::~Ghost()
{
}

void Ghost::OnBind()
{
	switch (m_name) {
	case BLINKY:
		spawnPos = m_map->GetWorldCoordinates({ 13, 4 });
		break;
	case PINKY:
		spawnPos = m_map->GetWorldCoordinates({ 16, 4 });
		break;
	case INKY:
		spawnPos = m_map->GetWorldCoordinates({ 19, 4 });
		break;
	case CLYDE:
		spawnPos = m_map->GetWorldCoordinates({ 22, 4 });
		break;
	default:
		break;
	}

	m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, GHOST_SPEITE_WIDTH, GHOST_SPEITE_WIDTH));

	Physics::SetObjectProperties(GetRealmID(), m_model_ID, ppRigid);
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetFriction(GetRealmID(), m_model_ID, 0);
}

void Ghost::OnRelease()
{
}

void Ghost::OnProcessInput(const float deltaTIme)
{
}

void Ghost::OnFinalUpdate()
{
}

void Ghost::OnRender()
{
	Renderer::Begin(GHOST_LAYER);
	Renderer::SetPolygonTexture("ghost", m_frame_col, m_frame_row);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}

void Ghost::AddMapPtr(Map* nMap)
{
	m_map = nMap;
}
