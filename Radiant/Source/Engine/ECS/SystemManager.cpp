#include "pch.h"
#include "SystemManager.h"
#include "EntityManager.h"

namespace rdt {
	System::System()
	{
	}

	void System::AddEntity(Entity entity)
	{
		m_entities.insert(entity);
	}

	void System::RemoveEntity(Entity entity)
	{
		m_entities.erase(entity);
	}

	bool System::HasEntity(Entity entity)
	{
		return m_entities.contains(entity);
	}

	bool System::UsesComponent(ComponentID component)
	{
		return m_signature.test(component);
	}

	void System::SetSignature(const Signature& signature)
	{
		m_signature = signature;
	}

	const std::set<Entity>& System::GetEntities()
	{
		return m_entities;
	}

	// ===================================================
	SystemManager* SystemManager::m_instance = nullptr;

	SystemManager::SystemManager()
	{
	}

	SystemManager::~SystemManager()
	{
		for (auto& [id, system] : m_instance->m_systems) {
			delete system;
		}
	}
	void SystemManager::Initialize()
	{
		Destroy();
		m_instance = new SystemManager;
	}

	void SystemManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	void SystemManager::OnProcessInput(float deltaTime)
	{
		for (auto system : m_instance->m_process_input_systems) {
			system->Update(deltaTime);
		}
	}

	void SystemManager::OnWorldUpdate(float deltaTime)
	{
		for (auto system : m_instance->m_update_world_systems) {
			system->Update(deltaTime);
		}
	}

	void SystemManager::OnFinalUpdate()
	{
		for (auto system : m_instance->m_final_update_systems) {
			system->Update();
		}
	}

	void SystemManager::OnRender()
	{
		for (auto system : m_instance->m_render_systems) {
			system->Update();
		}
	}

	void SystemManager::RegisterSystemImpl(const char* name, System* nSystem)
	{
		if (m_systems.find(name) != m_systems.end()) {
			RDT_CORE_WARN("SystemManager - Tried to register system '{}'", name);
			delete nSystem;
			return;
		}

		m_systems[name] = nSystem;
		AddToSystemQueue(nSystem);
	}

	void SystemManager::AddEntityImpl(const char* typeName, Entity entity)
	{
		if (m_systems.find(typeName) == m_systems.end()) {
			RDT_CORE_WARN("SystemManager - Tried to add entity to unregistered system '{}'", typeName);
			return;
		}

		m_systems.at(typeName)->AddEntity(entity);
	}

	void SystemManager::AddToSystemQueue(System* nSystem)
	{
		switch (nSystem->GetType()) {
		case SystemType::SystemNAT:
			RDT_CORE_WARN("SystemManager - Tried to add a system with NAT. Not added to any queues.");
			return;
		case SystemType::SystemProcessInput:
			m_process_input_systems.push_back(nSystem);
			return;
		case SystemType::SystemWorldUpdate:
			m_update_world_systems.push_back(nSystem);
			return;
		case SystemType::SystemFinalUpdate:
			m_final_update_systems.push_back(nSystem);
			return;
		case SystemType::SystemRenderUpdate:
			m_render_systems.push_back(nSystem);
			return;
		default:
			RDT_CORE_WARN("SystemManager - Tried to add system with unknown type [{}].", nSystem->GetType());
			return;
		}
	}

	void SystemManager::OnEntityComponentRemoved(Entity eID, ComponentID cID)
	{
		for (auto& [id, system] : m_instance->m_systems) {
			if (system->HasEntity(eID) && system->UsesComponent(cID)) {
				system->RemoveEntity(eID);
			}
		}
	}

	void SystemManager::OnEntityRemoved(Entity eID)
	{
		for (auto& [id, system] : m_instance->m_systems) {
			if (system->HasEntity(eID)) {
				system->RemoveEntity(eID);
			}
		}
	}

}