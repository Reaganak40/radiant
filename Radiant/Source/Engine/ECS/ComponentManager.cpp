#include "pch.h"
#include "ComponentManager.h"

namespace rdt {
	ComponentManager* ComponentManager::m_instance = nullptr;

	ComponentManager::ComponentManager()
	{
		m_componentCounter = 0;
	}

	ComponentManager::~ComponentManager()
	{
	}

	void ComponentManager::Initialize()
	{
		Destroy();
		m_instance = new ComponentManager;
	}
	void ComponentManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
}