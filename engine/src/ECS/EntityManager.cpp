#include "pch.h"
#include <Radiant/ECS/EntityManager.hpp>

#include <Radiant/Logger.hpp>

using Signature = std::bitset<RDT_MAX_COMPONENTS>;


struct rdt::ecs::EntityManager::Impl {

	std::unordered_map<std::string, EntityID> aliasToid;
	std::unordered_map<EntityID, std::string> idToAlias;

	EntityID idCounter;
	std::vector<Signature> m_signatures;
	std::vector<EntityID> m_id_cache;

	Impl()
	{
		// don't use first index;
		m_signatures.push_back({});
	}

	~Impl()
	{
	}

	EntityID NextEntityID()
	{
		EntityID nID;
		if (m_id_cache.size()) {
			nID = m_id_cache.back();
			m_id_cache.pop_back();
		}
		else {
			nID = ++idCounter;
		}
	}

	EntityID RegisterEntity(const char* name)
	{
		if (aliasToid.find(name) != aliasToid.end())
		{
			RDT_CORE_WARN("EntityManager - Could not register entity '{}', already exists.", name);
			return RDT_NULL_ENTITY_ID;
		}

		EntityID nID = NextEntityID();
		aliasToid[name] = nID;
		idToAlias[nID] = name;

		if (nID == m_signatures.size()) {
			m_signatures.push_back({});
		}
		else {
			m_signatures[nID].reset();
		}

		return nID;
	}

	void RemoveEntity(EntityID eID)
	{
		if (idToAlias.find(eID) == idToAlias.end())
		{
			RDT_CORE_WARN("EntityManager - Could not remove entity [id: {}], not found.", eID);
			return;
		}

		aliasToid.erase(idToAlias.at(eID));
		idToAlias.erase(eID);
		m_id_cache.push_back(eID);

		for (size_t bit = 0; bit < RDT_MAX_COMPONENTS; bit++)
		{
			if (m_signatures[eID].test(bit))
			{
				ComponentManager::RemoveFromComponent(eID, (ComponentID)bit);
			}
		}
	}

	void AddToSignature(EntityID eID, ComponentID cID)
	{
		if (m_signatures.size() <= eID) {
			RDT_CORE_WARN("EntityManager - Could not add to signature entityID [{}] out of range!", eID);
			return;
		}
		m_signatures[eID].set(cID, true);
	}
};

// ====================================================================


void rdt::ecs::EntityManager::Initialize()
{
	Destroy();
	m_impl = new Impl;
}

void rdt::ecs::EntityManager::Destroy()
{
	if (m_impl != nullptr) {
		delete m_impl;
		m_impl = nullptr;
	}
}

rdt::EntityID rdt::ecs::EntityManager::RegisterEntity(const char* alias)
{
	return m_impl->RegisterEntity(alias);
}

void rdt::ecs::EntityManager::RemoveEntity(EntityID entity)
{
	m_impl->RemoveEntity(entity);
}

void rdt::ecs::EntityManager::AddToSignature(EntityID entity, ComponentID cID)
{
	m_impl->AddToSignature(entity, cID);
}