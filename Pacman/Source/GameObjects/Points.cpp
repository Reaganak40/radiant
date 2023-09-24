#include "Points.h"

using namespace rdt;

Points::Points()
	: m_show_timer(1.0f)
{
	spawnPos = { FRUIT_POS_X, FRUIT_POS_Y };
	shouldShow = false;
	m_atlasX = 0;
	m_atlasY = 0;
	m_color = WHITE;

	RegisterToMessageBus("points");
}

Points::~Points()
{
}

void Points::OnBind()
{
	AddObjectToWorld(std::make_shared<Rect>(spawnPos, 48, 21));
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, NoCollision);
	Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());
}

void Points::OnRelease()
{
}

void Points::OnProcessInput(const float deltaTime)
{
	if (m_show_timer.IsRunning()) {
		if (m_show_timer.Update(deltaTime)) {
			shouldShow = false;
		}
	}
}

void Points::OnFinalUpdate()
{
}

void Points::OnRender()
{
	if (!shouldShow) {
		return;
	}

	Renderer::Begin();
	Renderer::SetPolygonTexture("points", m_atlasX, m_atlasY);
	Renderer::SetPolygonColor(m_color);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}

void Points::OnMessage(rdt::Message msg)
{
	switch (msg.type) {
	case PMT_ShowPointsEarned:
		OnShow((PointData*)msg.data);
		break;
	}
}

void Points::OnShow(PointData* data)
{
	SetPointVal(data->pointVal);
	
	spawnPos = data->pos;
	Physics::SetPosition(GetRealmID(), m_model_ID, spawnPos);
	
	shouldShow = true;
	m_show_timer.Start();
}

void Points::SetPointVal(unsigned int nVal)
{
	m_pointVal = nVal;
	SetAtlasCoordinates();

	if (m_atlasX == 0) {
		m_color = Color(39, 170, 166, 255, false);
	}
	else {
		m_color = WHITE;
	}
}

void Points::SetAtlasCoordinates()
{
	switch (m_pointVal) {
	case 200:
		m_atlasY = 0;
		m_atlasX = 0;
		break;
	case 400:
		m_atlasY = 1;
		m_atlasX = 0;
		break;
	case 800:
		m_atlasY = 2;
		m_atlasX = 0;
		break;
	case 1600:
		m_atlasY = 3;
		m_atlasX = 0;
		break;
	case 100:
		m_atlasY = 0;
		m_atlasX = 1;
		break;
	case 300:
		m_atlasY = 1;
		m_atlasX = 1;
		break;
	case 500:
		m_atlasY = 2;
		m_atlasX = 1;
		break;
	case 700:
		m_atlasY = 3;
		m_atlasX = 1;
		break;
	case 1000:
		m_atlasY = 4;
		m_atlasX = 1;
		break;
	case 2000:
		m_atlasY = 5;
		m_atlasX = 1;
		break;
	case 3000:
		m_atlasY = 6;
		m_atlasX = 1;
		break;
	case 5000:
		m_atlasY = 7;
		m_atlasX = 1;
		break;
	default:
		m_atlasY = 7;
		m_atlasX = 0;
		break;
	}
}
