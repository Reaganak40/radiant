#include "pch.h"
#include "GameObject.h"

namespace rdt {
	GameObject::GameObject()
		: m_ID(GetUniqueID()), m_sceneID(0), m_model_ID(0), m_realmID(0), m_gameState(0)
	{
	}
	void GameObject::OnMessage(Message msg)
	{
		printf("Warning: Using Base Class OnMessage!\n");
	}
}
