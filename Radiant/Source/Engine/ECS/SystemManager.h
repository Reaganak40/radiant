#pragma once
#include "Core.h"
#include "ECSTypes.h"
#include "ComponentManager.h"

namespace rdt {

	enum SystemType {
		SystemNAT,			// NOT A TYPE
		SystemProcessInput, // System for OnProcessInput()
		SystemWorldUpdate,  // System for OnWorldUpdate()
		SystemFinalUpdate,	// System for OnFinalUpdate()
		SystemRenderUpdate, // System for OnRender()
	};
	class System {
	private:
		std::set<Entity> m_entities;
		Signature m_signature;

	public:
		System();
		virtual ~System() = default;
		virtual SystemType GetType() { return SystemNAT; }

		/*
			To implement function that contains the logic for the system
		*/
		virtual void Update(float deltaTime = 0) = 0;

		/*
			Adds an entity to the this system.
		*/
		void AddEntity(Entity entity);

		/*
			Removes an entity from this system, if it exists.
		*/
		void RemoveEntity(Entity entity);

		/*
			Returns true if the entity is currently being used by this system.
		*/
		bool HasEntity(Entity entity);

		/*
			Returns true if this system utilizes the provided component
		*/
		bool UsesComponent(ComponentID component);

	protected:

		/*
			Sets the component signature for this sysem. Indicates what components
			this system utilizes when it calls Update(). This should be called in
			the child class constructor.
		*/
		void SetSignature(const Signature& signature);

		/*
			Gets the set of entities that are registered to this system.
		*/
		const std::set<Entity>& GetEntities();

		/*
			Gets a component (array) from the ComponentManager if it exists and it matches
			this system signature.
		*/
		template<typename T>
		Component<T>* GetComponent()
		{
			ComponentID cID = ComponentManager::GetComponentID<T>();

			if (cID == NOT_REGISTERED_COMPONENT) {
				const char* typeName = typeid(T).name();
				RDT_CORE_ERROR("System - Tried to get component '{}' but it is not registered.", typeName);
				return nullptr;
			}

			if (!m_signature[cID]) {
				const char* typeName = typeid(T).name();
				RDT_CORE_ERROR("System - Tried to get component '{}' but does not match this system's signature.", typeName);
				return nullptr;
			}

			return ComponentManager::GetComponent<T>();
		}
	};

	namespace systems {
		// =====================================================
		class ProcessInput : public System {
		public:
			ProcessInput() {}
			~ProcessInput() {}
			SystemType GetType() override final { return SystemProcessInput; }
			virtual void Update(float deltaTime = 0) { RDT_CORE_WARN("System::ProcessInput - virtual update called."); }
		};
		// =====================================================
		class WorldUpdate : public System {
		public:
			WorldUpdate() {}
			~WorldUpdate() {}
			SystemType GetType() override final { return SystemWorldUpdate; }
			virtual void Update(float deltaTime = 0) { RDT_CORE_WARN("System::WorldUpdate - virtual update called."); }
		};
		// =====================================================
		class FinalUpdate : public System {
		public:
			FinalUpdate() {}
			~FinalUpdate() {}
			SystemType GetType() override final { return SystemFinalUpdate; }
			virtual void Update(float deltaTime = 0) { RDT_CORE_WARN("System::FinalUpdate - virtual update called."); }
		};
		// =====================================================
		class RenderUpdate : public System {
		public:
			RenderUpdate() {}
			~RenderUpdate() {}
			SystemType GetType() override final { return SystemRenderUpdate; }
			virtual void Update(float deltaTime = 0) { RDT_CORE_WARN("System::RenderUpdate - virtual update called."); }
		};
		// =====================================================
	}

	class RADIANT_API SystemManager {
	private:
		SystemManager();
		~SystemManager();
		static SystemManager* m_instance;

		std::unordered_map<std::string, System*> m_systems;
		std::vector<System*> m_process_input_systems;
		std::vector<System*> m_update_world_systems;
		std::vector<System*> m_final_update_systems;
		std::vector<System*> m_render_systems;

	public:

		static void Initialize();
		static void Destroy();

		/*
			Registers a new system to the SystemManager
		*/
		template<typename T>
		static void RegisterSystem()
		{
			const char* typeName = typeid(T).name();
			m_instance->RegisterSystemImpl(typeName, new T);
		}

		/*
			Adds an entity to the system T
		*/
		template<typename T>
		static void AddEntity(Entity entity)
		{
			const char* typeName = typeid(T).name();
			m_instance->AddEntityImpl(typeName, entity);
		}

		/*
			Calls Update() on all ProcessInput systems
		*/
		static void OnProcessInput(float deltaTime);

		/*
			Calls Update() on all WorldUpdate systems
		*/
		static void OnWorldUpdate(float deltaTime);

		/*
			Calls Update() on all final update systems
		*/
		static void OnFinalUpdate();

		/*
			Calleds Update() on all render update systems
		*/
		static void OnRender();

		friend class EntityManager;
	private:
		void RegisterSystemImpl(const char* name, System* nSystem);
		void AddEntityImpl(const char* typeName, Entity entity);
		
		void AddToSystemQueue(System* nSystem);
		static void OnEntityComponentRemoved(Entity eID, ComponentID cID);
		static void OnEntityRemoved(Entity eID);


	};
}