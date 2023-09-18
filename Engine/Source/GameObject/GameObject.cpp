#include "pch.h"
#include "GameObject.h"
#include "Physics/Physics.h"

namespace rdt {
	GameObject::GameObject()
		: m_ID(GetUniqueID()), m_sceneID(0), m_model_ID(0), m_realmID(0)
	{
	}

	void GameObject::AddObjectToWorld(std::shared_ptr<Polygon> polygon)
	{
		m_model_ID = Physics::CreateObject(m_realmID, GetMessageID(), polygon);
	}

	void GameObject::OnMessage(Message msg)
	{
		printf("Warning: Using Base Class OnMessage!\n");
	}
}
