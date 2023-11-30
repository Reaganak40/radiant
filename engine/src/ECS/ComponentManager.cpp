#include "pch.h"
#include <Radiant/ECS/ComponentManager.hpp>
#include <Core/Utils/Error.hpp>

namespace rdt::ecs {
	class ComponentManagerImpl;
}

struct rdt::ecs::ComponentManagerImpl
{
	ComponentID idCounter = 0;
	std::unordered_map<ComponentType, ComponentID> m_componentTypes;
	std::array<IComponentArray*, RDT_MAX_COMPONENTS> m_components;

	ComponentID NextComponentID()
	{
		return idCounter++;
	}

	void RegisterComponent(ComponentType type, IComponentArray* component)
	{
		if (m_componentTypes.find(type) != m_componentTypes.end()) {
			RDT_CORE_WARN("ComponentManager - Tried to register duplicate component '{}'", type.name());
		}

		ComponentID nID = NextComponentID();

		if (nID == RDT_MAX_COMPONENTS) {
			RDT_THROW_COMPONENT_OVERFLOW_EXCEPTION("Exceeded Component Limit: {}", RDT_MAX_COMPONENTS);
		}

		m_componentTypes[type] = nID;
		m_components[nID] = component;
	}

	ComponentID GetComponentID(ComponentType type)
	{
		if (m_componentTypes.find(type) == m_componentTypes.end())
		{
			return RDT_NULL_COMPONENT_ID;
		}
		return m_componentTypes.at(type);
	}

	void* GetComponent(ComponentType type)
	{
		if (m_componentTypes.find(type) == m_componentTypes.end()) {
			RDT_CORE_WARN("ComponentManager - Could not find component '{}'", type.name());
			return nullptr;
		}

		return m_components[m_componentTypes.at(type)];
	}

	void RemoveFromComponent(EntityID eID, ComponentID cID)
	{
		m_components[cID]->RemoveData(eID);
	}
};

// ===================================================================

static rdt::ecs::ComponentManagerImpl* m_impl = nullptr;

void rdt::ecs::ComponentManager::Initialize()
{
	Destroy();
	m_impl = new ComponentManagerImpl;
}

void rdt::ecs::ComponentManager::Destroy()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}

void rdt::ecs::ComponentManager::RegisterComponentImpl(ComponentType type, IComponentArray* component)
{
	m_impl->RegisterComponent(type, component);
}

rdt::ComponentID rdt::ecs::ComponentManager::GetComponentID(ComponentType type)
{
	return m_impl->GetComponentID(type);
}

void* rdt::ecs::ComponentManager::GetComponent(ComponentType type)
{
	return m_impl->GetComponent(type);
}

void rdt::ecs::ComponentManager::RemoveFromComponent(EntityID eID, ComponentID cID)
{
	m_impl->RemoveFromComponent(eID, cID);
}
