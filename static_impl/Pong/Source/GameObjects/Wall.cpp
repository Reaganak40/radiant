#include "Wall.h"

Wall::Wall(double xPos, double yPos)
{
	m_is_visible = false;

	spawnPos = rdt::Vec2d(xPos, yPos);
}

Wall::~Wall()
{
}

void Wall::OnBind()
{
	using namespace rdt;
	m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, WALL_WIDTH, WALL_HEIGHT));

	Physics::SetObjectProperties(GetRealmID(), m_model_ID, NoCollision);
}

void Wall::OnProcessInput(const float deltaTIme)
{
}

void Wall::OnFinalUpdate()
{
}

void Wall::OnRender()
{
	using namespace rdt;
	if (m_is_visible) {
		Renderer::Begin(WALL_LAYER);
		Renderer::SetPolygonColor(WHITE);
		Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
		Renderer::End();
	}
}

void Wall::SetWallVisibility(bool isVisible)
{
	m_is_visible = isVisible;
}
