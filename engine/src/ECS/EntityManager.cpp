#include "pch.h"
#include "EntityManager.h"

#include <Radiant/Logger.hpp>

rdt::ecs::EntityManager* rdt::ecs::EntityManager::m_instance = nullptr;

rdt::ecs::EntityManager::EntityManager()
{
}

rdt::ecs::EntityManager::~EntityManager()
{
}

rdt::ecs::EntityManager& rdt::ecs::EntityManager::Get()
{
	if (m_instance == nullptr) {
		m_instance = new EntityManager();
	}

	return *m_instance;
}

void rdt::ecs::EntityManager::Destroy()
{
	if (m_instance != nullptr) {
		delete m_instance;
		m_instance = nullptr;
	}
}

EntityID rdt::ecs::EntityManager::RegisterEntity(const std::string& alias)
{
	return EntityID();
}

void rdt::ecs::EntityManager::RemoveEntity(EntityID entity)
{
}

void rdt::ecs::EntityManager::SetSignature(EntityID entity, const Signature& signature)
{
}

Signature rdt::ecs::EntityManager::GetSignature(EntityID eID)
{
	return Signature();
}
