#include "Fruit.h"

using namespace rdt;

Fruit::Fruit()
	: m_show_timer(9.5f)
{
	m_type = CHERRY;
	m_shouldShow = false;
	m_eaten_once = false;
	m_pacman_ptr = nullptr;
	m_fruitEatenSound = 0;

	RegisterToMessageBus("fruit");
}

Fruit::~Fruit()
{
}

void Fruit::OnBind()
{
	AddObjectToWorld(std::make_shared<Rect>(Vec2d(FRUIT_POS_X, FRUIT_POS_Y), FRUIT_WIDTH, FRUIT_HEIGHT));
	Physics::SetObjectProperties(GetRealmID(), m_model_ID, DontResolve);
	Physics::AddPTag(GetRealmID(), m_model_ID, "pacman");
	Physics::SetHitBoxSize(GetRealmID(), m_model_ID, { 0.45, 0.45 });
	Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
	Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());

	SendMessage("pacman", MT_RequestGameObjectPtr, nullptr);

	m_fruitEatenSound = SoundEngine::CreateNewSound("fruitEaten", new SoundEffect);
}

void Fruit::OnRelease()
{
}

void Fruit::OnProcessInput(const float deltaTIme)
{
	if (m_show_timer.IsRunning()) {
		if (m_show_timer.Update(deltaTIme)) {
			m_shouldShow = false;
		}
	}
}

void Fruit::OnFinalUpdate()
{
}

void Fruit::OnRender()
{
	if (!m_shouldShow) {
		return;
	}

	Renderer::Begin(PAC_DOT_LAYER);
	Renderer::SetPolygonTexture("fruit", m_type, 0);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_ID));
	Renderer::End();
}

void Fruit::OnMessage(rdt::Message msg)
{
	switch (msg.type) {
	case MT_Collision:
		ResolveCollision((CollisionData*)msg.data);
		break;
	case MT_SendGameObjectPtr:
		AddGameObjectPtr(msg.from, (GameObjectPtrData*)msg.data);
		break;
	case PMT_ShowFruit:
		OnShowFruit();
		break;
	case PMT_Respawn:
		OnRespawn();
		break;
	case PMT_StartNewLevel:
		OnRespawn();
		m_eaten_once = false;
		break;
	case PMT_SetFruit:
		SetType(((FruitData*)msg.data)->type);
		break;
	}
}

void Fruit::SetType(FRUIT_TYPE type)
{
	m_type = type;
}

void Fruit::AddGameObjectPtr(rdt::MessageID from, rdt::GameObjectPtrData* data)
{
	std::string alias = MessageBus::GetAlias(from);

	if (alias == "pacman") {
		m_pacman_ptr = (Pacman*)data->ptr;
	}
}

void Fruit::ResolveCollision(rdt::CollisionData* data)
{
	UniqueID pacmanModelID = m_pacman_ptr->GetModelID();

	if (data->source == pacmanModelID && m_shouldShow) {
		SendMessage("level", PMT_FruitEaten, new FruitData(m_type, m_eaten_once));
		m_shouldShow = false;
		m_eaten_once = true;
		SoundEngine::PlaySound(m_fruitEatenSound);
	}
}

void Fruit::OnShowFruit()
{
	if (!m_shouldShow) {
		m_shouldShow = true;
		m_show_timer.Start();
	}
}

void Fruit::OnRespawn()
{
	if (m_shouldShow) {
		m_shouldShow = false;
		m_show_timer.End();
	}
}
