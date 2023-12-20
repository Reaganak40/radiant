#include "pch.h"
#include <Radiant/ECS/ComponentManager.hpp>
#include <Core/Utils/Error.hpp>

namespace rdt::ecs {
	class ComponentManagerImpl;
}

struct rdt::ecs::ComponentManagerImpl
{
	ComponentSlot idCounter = 0;
	std::unordered_map<ComponentID, ComponentSlot> m_componentTypes;
	std::array<IComponentArray*, RDT_MAX_COMPONENTS> m_components;

	ComponentSlot NextComponentSlot()
	{
		return idCounter++;
	}

	void RegisterComponent(ComponentID cID, IComponentArray* component)
	{
		if (m_componentTypes.find(cID) != m_componentTypes.end()) {
			RDT_CORE_WARN("ComponentManager - Tried to register duplicate component '{}'", cID.name());
			delete component;
			return;
		}

		ComponentSlot cSlot = NextComponentSlot();

		if (cSlot == RDT_MAX_COMPONENTS) {
			RDT_THROW_COMPONENT_OVERFLOW_EXCEPTION("Exceeded Component Limit: {}", RDT_MAX_COMPONENTS);
		}

		m_componentTypes[cID] = cSlot;
		m_components[cSlot] = component;
	}

	IComponentArray* GetComponentArray(ComponentID cID)
	{
		if (m_componentTypes.find(cID) == m_componentTypes.end()) {
			return nullptr;
		}

		return m_components.at(m_componentTypes.at(cID));
	}

	bool ComponentExists(ComponentID cID)
	{
		return m_componentTypes.find(cID) != m_componentTypes.end();
	}

	ComponentSlot GetComponentSlot(ComponentID cID)
	{
		if (!ComponentExists(cID)) {
			return RDT_NOT_REGISTERED_COMPONENT;
		}

		return m_componentTypes.at(cID);
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

void rdt::ecs::ComponentManager::RegisterComponentImpl(ComponentID cID, IComponentArray* component)
{
}

rdt::ecs::IComponentArray* rdt::ecs::ComponentManager::GetComponentArray(ComponentID cID)
{
	return m_impl->GetComponentArray(cID);
}

void rdt::ecs::ComponentManager::RemoveFromComponent(EntityID eID, ComponentID cID)
{
	m_impl->RemoveFromComponent(eID, cID);
}

bool rdt::ecs::ComponentManager::IsComponentRegistered(ComponentID cID)
{
	return m_impl->ComponentExists(cID);
}

ComponentSlot rdt::ecs::ComponentManager::GetComponentSlot(ComponentID cID)
{
	return ComponentSlot();
}
