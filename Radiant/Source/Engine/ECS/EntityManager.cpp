#include "pch.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "CommonComponents.h"
#include "Logging/Log.h"

namespace rdt {
	
	struct EntityManager::Impl {
		
		// EntityID management
		Entity m_idCounter;
		std::vector<Entity> m_idCache;

		std::unordered_map<Entity, Signature> m_signatures;

		Impl()
		{
			m_idCounter = NO_ENTITY_ID;
		}

		~Impl()
		{
		}

		Entity RegisterEntity()
		{
			Entity nID = NO_ENTITY_ID;
			if (m_idCache.size() > 0) {
				nID = m_idCache.back();
				m_idCache.pop_back();
			}
			else {
				nID = ++m_idCounter;
			}

			m_signatures[nID];

			return nID;
		}

		bool RemoveEntity(Entity eID)
		{
			if (m_signatures.find(eID) == m_signatures.end()) {
				RDT_CORE_WARN("EntityManager - Tried to remove entity [id:{}], but it doesn't exist.", eID);
				return false;
			}

			m_signatures.erase(eID);
			m_idCache.push_back(eID);
			return true;
		}

		bool EntityExists(Entity eID)
		{
			return m_signatures.find(eID) != m_signatures.end();
		}

		void SetSignature(Entity eID, const Signature& signature)
		{
			if (m_signatures.find(eID) == m_signatures.end()) {
				RDT_CORE_WARN("EntityManager - Tried to add signature to unregistered entity [id:{}]", eID);
				return;
			}

			m_signatures[eID] = signature;
		}

		const Signature& GetSignature(Entity eID)
		{
			// Precondition: Entity for sure exists
			return m_signatures.at(eID);
		}

		void AddToSignature(Entity eID, ComponentID cID)
		{
			if (m_signatures.find(eID) == m_signatures.end()) {
				RDT_CORE_WARN("EntityManager - Tried to add component to unregistered entity [id:{}]", eID);
				return;
			}
			
			m_signatures.at(eID).set(cID, true);
		}

		void RemoveFromSignature(Entity eID, ComponentID cID)
		{
			if (m_signatures.find(eID) == m_signatures.end()) {
				RDT_CORE_WARN("EntityManager - Tried to remove component to unregistered entity [id:{}]", eID);
				return;
			}

			m_signatures.at(eID).set(cID, false);
		}
	};


	// ========================================================

	EntityManager* EntityManager::m_instance = nullptr;

	EntityManager::EntityManager()
		: m_impl(new EntityManager::Impl)
	{
	}

	EntityManager::~EntityManager()
	{
		delete m_impl;
	}

	void EntityManager::Initialize()
	{
		Destroy();
		m_instance = new EntityManager;
	}

	void EntityManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
	
	Entity EntityManager::RegisterEntity(const std::string& alias, Layer* owner)
	{
		Entity nID = m_instance->m_impl->RegisterEntity();

		EntityConfig config;

		if (alias.empty()) {
			config.alias = "entity" + std::to_string(nID);
		}
		else {
			config.alias = alias;
		}
		config.owner = owner;

		EntityManager::AddComponent<EntityConfig>(nID, config);
		
		return nID;
	}

	void EntityManager::RemoveEntity(Entity eID)
	{
		if (!m_instance->m_impl->RemoveEntity(eID)) {
			return;
		}

		ComponentManager::OnEntityRemoved(eID);
		SystemManager::OnEntityRemoved(eID);
	}
	void EntityManager::SetSignature(Entity eID, const Signature& signature)
	{
		m_instance->m_impl->SetSignature(eID, signature);
	}

	Signature EntityManager::GetSignature(Entity eID)
	{
		if (!m_instance->m_impl->EntityExists(eID)) {
			RDT_CORE_WARN("EntityManager - Tried to get signature to unregistered entity [id:{}]", eID);
			return Signature();
		}

		return m_instance->m_impl->GetSignature(eID);
	}

	const std::unordered_map<Entity, Signature>& EntityManager::GetEntityMap()
	{
		return m_instance->m_impl->m_signatures;
	}

	const char* EntityManager::GetEntityAlias(Entity entity)
	{
		if (!EntityExists(entity)) {
			RDT_CORE_WARN("EntityManager - Tried to get alias of an unregistered entity [id: {}]", entity);
			return nullptr;
		}

		return GetComponent<EntityConfig>(entity)->alias.c_str();
	}

	void EntityManager::AddToSignature(Entity eID, ComponentID cID)
	{
		m_instance->m_impl->AddToSignature(eID, cID);
	}

	void EntityManager::RemoveFromSignature(Entity eID, ComponentID cID)
	{
		m_instance->m_impl->RemoveFromSignature(eID, cID);
	}

	bool EntityManager::EntityExists(Entity eID)
	{
		return m_instance->m_impl->m_signatures.find(eID) != m_instance->m_impl->m_signatures.end();
	}

	bool EntityManager::HasComponentImpl(Entity eID, ComponentID cID)
	{
		return m_impl->m_signatures.at(eID)[cID];
	}

}
