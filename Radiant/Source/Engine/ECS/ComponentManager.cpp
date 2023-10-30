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
		for (auto& [id, component] : m_components) {
			delete component;
		}
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

	void ComponentManager::OnEntityRemoved(Entity eID)
	{
		for (auto& [id, component] : m_instance->m_components) {
			component->RemoveData(eID);
		}
	}

	ComponentID ComponentManager::GetComponentIDImpl(const char* typeName)
	{
		if (m_componentTypes.find(typeName) == m_componentTypes.end()) {
			RDT_CORE_WARN("ComponentManager - Could not get component type for '{}'", typeName);
			return NOT_REGISTERED_COMPONENT;
		}

		return m_componentTypes.at(typeName);
	}

}