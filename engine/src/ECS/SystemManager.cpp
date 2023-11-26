#include "pch.h"
#include "SystemManager.h"
#include "EntityManager.h"

namespace rdt {

	struct System::Impl {
		std::set<Entity> m_entities;
		Signature m_signature;

		Impl()
		{
		}

		~Impl()
		{
		}

	};
	System::System()
		: m_impl(new System::Impl)
	{
	}

	System::~System()
	{
		delete m_impl;
	}

	void System::AddEntity(Entity entity)
	{
		m_impl->m_entities.insert(entity);
	}

	void System::RemoveEntity(Entity entity)
	{
		m_impl->m_entities.erase(entity);
	}

	bool System::HasEntity(Entity entity)
	{
		return m_impl->m_entities.contains(entity);
	}

	bool System::UsesComponent(ComponentID component)
	{
		return m_impl->m_signature.test(component);
	}

	void System::SetSignature(const Signature& signature)
	{
		m_impl->m_signature = signature;
	}

	const std::set<Entity>& System::GetEntities()
	{
		return m_impl->m_entities;
	}

	// ===================================================
	struct SystemManager::Impl
	{
		std::unordered_map<std::string, System*> m_systems;
		std::vector<System*> m_process_input_systems;
		std::vector<System*> m_update_world_systems;
		std::vector<System*> m_final_update_systems;
		std::vector<System*> m_render_systems;

		Impl()
		{
		}

		~Impl()
		{
			for (auto& [id, system] : m_systems) {
				delete system;
			}
		}

		void RegisterSystem(const char* name, System* nSystem)
		{
			if (m_systems.find(name) != m_systems.end()) {
				RDT_CORE_WARN("SystemManager - Tried to register system '{}'", name);
				delete nSystem;
				return;
			}

			m_systems[name] = nSystem;
		}

		void AddToSystemQueue(System* nSystem)
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

		void AddEntity(const char* typeName, Entity entity)
		{
			if (m_systems.find(typeName) == m_systems.end()) {
				RDT_CORE_WARN("SystemManager - Tried to add entity to unregistered system '{}'", typeName);
				return;
			}

			m_systems.at(typeName)->AddEntity(entity);
		}

		void OnEntityRemoved(Entity eID)
		{
			for (auto& [id, system] : m_systems) {
				if (system->HasEntity(eID)) {
					system->RemoveEntity(eID);
				}
			}
		}
	};
	// =================================================================

	SystemManager* SystemManager::m_instance = nullptr;

	SystemManager::SystemManager()
		: m_impl(new SystemManager::Impl)
	{
	}

	SystemManager::~SystemManager()
	{
		delete m_impl;
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
		for (auto system : m_instance->m_impl->m_process_input_systems) {
			system->Update(deltaTime);
		}
	}

	void SystemManager::OnWorldUpdate(float deltaTime)
	{
		for (auto system : m_instance->m_impl->m_update_world_systems) {
			system->Update(deltaTime);
		}
	}

	void SystemManager::OnFinalUpdate()
	{
		for (auto system : m_instance->m_impl->m_final_update_systems) {
			system->Update();
		}
	}

	void SystemManager::OnRender()
	{
		for (auto system : m_instance->m_impl->m_render_systems) {
			system->Update();
		}
	}

	const std::unordered_map<std::string, System*>& SystemManager::GetSystemMap()
	{
		return m_instance->m_impl->m_systems;
	}

	void SystemManager::RegisterSystemImpl(const char* name, System* nSystem)
	{
		m_impl->RegisterSystem(name, nSystem);
		m_impl->AddToSystemQueue(nSystem);
	}

	void SystemManager::AddEntityImpl(const char* typeName, Entity entity)
	{
		m_impl->AddEntity(typeName, entity);
	}

	void SystemManager::OnEntityRemoved(Entity eID)
	{
		m_instance->m_impl->OnEntityRemoved(eID);
	}

}