#include "Wall.h"

Wall::Wall(double xPos, double yPos)
{
	m_is_visible = false;

	spawnPos = Radiant::Vec2d(xPos, yPos);
}

Wall::~Wall()
{
}

void Wall::OnRegister()
{
	using namespace Radiant;
	m_model_ID = Physics::CreateObject(std::make_shared<Rect>(spawnPos, WALL_WIDTH, WALL_HEIGHT));

	Physics::SetObjectProperties(m_model_ID, ppRigid);
}

void Wall::OnProcessInput(const float deltaTIme)
{
}

void Wall::OnFinalUpdate()
{
}

void Wall::OnRender()
{
	using namespace Radiant;
	if (m_is_visible) {
		Renderer::Begin(WALL_LAYER);
		Renderer::SetPolygonColor(BLACK);
		Renderer::AddPolygon(Physics::GetPolygon(m_model_ID));
		Renderer::End();
	}
}

void Wall::SetWallVisibility(bool isVisible)
{
	m_is_visible = isVisible;
}
