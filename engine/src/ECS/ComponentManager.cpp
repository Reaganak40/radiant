#include "pch.h"
#include "ComponentManager.h"

namespace rdt {

	struct ComponentManager::Impl {
		ComponentID idCounter;
		std::unordered_map<std::string, ComponentID> m_componentTypes;
		std::unordered_map<std::string, IComponentArray*> m_components;

		Impl()
		{
			idCounter = 0;
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

		ComponentID GenerateID() {
			return idCounter++;
		}

		ComponentID GetComponentID(const char* typeName)
		{
			if (m_componentTypes.find(typeName) == m_componentTypes.end()) {
				RDT_CORE_WARN("ComponentManager - Could not get component type for '{}'", typeName);
				return NOT_REGISTERED_COMPONENT;
			}

			return m_componentTypes.at(typeName);
		}

		void* GetData(ComponentID cID, Entity entity) {

			for (auto& [componentName, id] : m_componentTypes) {
				if (id == cID) {
					return m_components.at(componentName)->GetDataPtr(entity);
				}
			}

			RDT_CORE_WARN("ComponentManager - Could not find component '{}'", cID);
			return nullptr;
		}

		void RegisterComponent(const char* typeName, IComponentArray* component)
		{
			m_componentTypes[typeName] = GenerateID();
			m_components[typeName] = component;
		}

		IComponentArray* GetComponentByID(ComponentID cID)
		{
			for (auto& [componentName, id] : m_componentTypes) {
				if (id == cID) {
					return m_components.at(componentName);
				}
			}

			return nullptr;
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

	void* ComponentManager::GetData(ComponentID cID, Entity entity)
	{
		return m_instance->m_impl->GetData(cID, entity);
	}

	const char* ComponentManager::GetComponenentName(ComponentID cID)
	{
		for (auto& [name, componentID] : m_instance->m_impl->m_componentTypes) {
			if (componentID == cID) {
				return name.c_str();
			}
		}

		RDT_CORE_WARN("ComponentManager - Could not find component with id: {}", cID);
		return nullptr;
	}

	bool ComponentManager::IsHiddenComponent(const std::string& name)
	{
		return (name == "struct rdt::EntityConfig") || (name == "struct rdt::DebugComponent");
	}

	size_t ComponentManager::GetComponentCount()
	{
		return m_instance->m_impl->idCounter;
	}

	IComponentArray* ComponentManager::GetComponentByID(ComponentID cID)
	{
		return m_instance->m_impl->GetComponentByID(cID);
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
		return m_impl->idCounter == MAX_COMPONENTS;
	}

	void ComponentManager::RegisterComponentImpl(const char* typeName, IComponentArray* component)
	{
		m_impl->RegisterComponent(typeName, component);
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