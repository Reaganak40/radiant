#include "pch.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "Logging/Log.h"

namespace rdt {
	
	struct EntityManager::Impl {
		
		// EntityID management
		EntityID m_idCounter;
		std::vector<EntityID> m_idCache;

		std::unordered_map<EntityID, Signature> m_signatures;

		Impl()
		{
			m_idCounter = NO_ENTITY_ID;
		}

		~Impl()
		{
		}

		EntityID RegisterEntity()
		{
			EntityID nID = NO_ENTITY_ID;
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

		bool RemoveEntity(EntityID eID)
		{
			if (m_signatures.find(eID) == m_signatures.end()) {
				RDT_CORE_WARN("EntityManager - Tried to remove entity [id:{}], but it doesn't exist.", eID);
				return false;
			}

			m_signatures.erase(eID);
			m_idCache.push_back(eID);
			return true;
		}

		bool EntityExists(EntityID eID)
		{
			return m_signatures.find(eID) != m_signatures.end();
		}

		void SetSignature(EntityID eID, const Signature& signature)
		{
			if (m_signatures.find(eID) == m_signatures.end()) {
				RDT_CORE_WARN("EntityManager - Tried to add signature to unregistered entity [id:{}]", eID);
				return;
			}

			m_signatures[eID] = signature;
		}

		const Signature& GetSignature(EntityID eID)
		{
			// Precondition: Entity for sure exists
			return m_signatures.at(eID);
		}

		void AddToSignature(EntityID eID, ComponentID cID)
		{
			if (m_signatures.find(eID) == m_signatures.end()) {
				RDT_CORE_WARN("EntityManager - Tried to add component to unregistered entity [id:{}]", eID);
				return;
			}
			
			m_signatures.at(eID).set(cID, true);
		}

		void RemoveFromSignature(EntityID eID, ComponentID cID)
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
	
	EntityID EntityManager::RegisterEntity()
	{
		return m_instance->m_impl->RegisterEntity();
	}

	void EntityManager::RemoveEntity(EntityID eID)
	{
		if (!m_instance->m_impl->RemoveEntity(eID)) {
			return;
		}

		ComponentManager::OnEntityRemoved(eID);
		SystemManager::OnEntityRemoved(eID);
	}
	void EntityManager::SetSignature(EntityID eID, const Signature& signature)
	{
		m_instance->m_impl->SetSignature(eID, signature);
	}

	Signature EntityManager::GetSignature(EntityID eID)
	{
		if (!m_instance->m_impl->EntityExists(eID)) {
			RDT_CORE_WARN("EntityManager - Tried to get signature to unregistered entity [id:{}]", eID);
			return Signature();
		}

		return m_instance->m_impl->GetSignature(eID);
	}

	void EntityManager::AddToSignature(EntityID eID, ComponentID cID)
	{
		m_instance->m_impl->AddToSignature(eID, cID);
	}

	void EntityManager::RemoveFromSignature(EntityID eID, ComponentID cID)
	{
		m_instance->m_impl->RemoveFromSignature(eID, cID);
		SystemManager::OnEntityComponentRemoved(eID, cID);
	}
}
