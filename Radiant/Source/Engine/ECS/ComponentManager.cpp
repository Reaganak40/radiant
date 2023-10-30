#include "pch.h"
#include "ComponentManager.h"

namespace rdt {

	struct ComponentManager::Impl {
		ComponentID m_componentCounter;
		std::unordered_map<std::string, ComponentID> m_componentTypes;
		std::unordered_map<std::string, IComponentArray*> m_components;

		Impl()
		{
			m_componentCounter = 0;
		}

		~Impl()
		{
			for (auto& [id, component] : m_components) {
				delete component;
			}
		}

		void OnEntityRemoved(Entity eID)
		{
			for (auto& [id, component] : m_components) {
				component->RemoveData(eID);
			}
		}

		ComponentID GetComponentID(const char* typeName)
		{
			if (m_componentTypes.find(typeName) == m_componentTypes.end()) {
				RDT_CORE_WARN("ComponentManager - Could not get component type for '{}'", typeName);
				return NOT_REGISTERED_COMPONENT;
			}

			return m_componentTypes.at(typeName);
		}

	};
	// =================================================================

	ComponentManager* ComponentManager::m_instance = nullptr;

	ComponentManager::ComponentManager()
		: m_impl(new ComponentManager::Impl)
	{
	}

	ComponentManager::~ComponentManager()
	{
		delete m_impl;
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
		m_instance->m_impl->OnEntityRemoved(eID);
	}

	bool ComponentManager::ComponentExists(const char* typeName)
	{
		return m_impl->m_components.find(typeName) != m_impl->m_components.end();
	}

	bool ComponentManager::MaxComponentsReached()
	{
		return m_impl->m_componentCounter == MAX_COMPONENTS;
	}

	void ComponentManager::RegisterComponentImpl(const char* typeName, IComponentArray* component)
	{
		m_impl->m_componentTypes[typeName] = m_impl->m_componentCounter++;
		m_impl->m_components[typeName] = component;
	}

	void* ComponentManager::GetComponentImpl(const char* typeName)
	{
		return m_impl->m_components.at(typeName);
	}

	ComponentID ComponentManager::GetComponentIDImpl(const char* typeName)
	{
		return m_impl->GetComponentID(typeName);
	}

}