/*******************************************************************
*	Module:  ECS
*	File:    ComponentManager.h
*
*	Author: Reagan Kelley
*
*   The file contains declarations and definitions for ECS 
*   components. Components hold entity data, and posses unique
*   IDs controlled by the ComponentManager for client access.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Delcarations
namespace rdt::core {
	class Realm;
}

#include "ECSTypes.h"
#include "Logging/Log.h"

namespace rdt {

	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;

		virtual void RemoveData(Entity eID) = 0;
		virtual void* GetDataPtr(Entity eID) = 0;
	};

	template <typename T>
	class Component : public IComponentArray {
	private:

		std::unordered_map<Entity, size_t> m_entity_map;

		std::vector<Entity> m_entities;
		std::vector<T> m_data;
	public:

		/*
			Adds data to the component array while tracking the associated
			entity.
		*/
		void InsertData(Entity eID, const T& data) {
			if (m_entity_map.find(eID) != m_entity_map.end()) {
				RDT_CORE_WARN("Component - Tried to insert duplicate entity [eID: {}]", eID);
				return;
			}

			m_entity_map[eID] = m_data.size();
			m_entities.push_back(eID);
			m_data.push_back(data);
		}

		/*
			Removes the data from this component data array that is associated with this
			Entity.
		*/
		void RemoveData(Entity eID) override final {
			if (m_entity_map.find(eID) == m_entity_map.end()) {
				RDT_CORE_WARN("Component - Could not remove data, entity not found [eID: {}]", eID);
				return;
			}

			size_t index = m_entity_map.at(eID);
			m_data.erase(m_data.begin() + index);
			m_entities.erase(m_entities.begin() + index);
			m_entity_map.erase(eID);

			// Update the data index location of all entities using this component
			for (size_t i = index; i < m_entities.size(); i++) {
				m_entity_map.at(m_entities[i]) = i;
			}
		}

		T& GetData(Entity eID) {
			return m_data[m_entity_map.at(eID)];
		}

		bool HasEntity(Entity eID) {
			return m_entity_map.find(eID) != m_entity_map.end();
		}

		void* GetDataPtr(Entity eID) override final {

			if (!HasEntity(eID)) {
				return nullptr;
			}

			return &m_data[m_entity_map.at(eID)];
		}
	};
	// ====================================================================

	class RADIANT_API ComponentManager {
	private:
		struct Impl;
		Impl* m_impl;

		ComponentManager();
		~ComponentManager();
		static ComponentManager* m_instance;
	public:

		/*
			Create a new singleton instance of the EntityManager
		*/
		static void Initialize();

		/*
			Destroy the current singleton instance of the EntityManager
		*/
		static void Destroy();

		template<typename T>
		static void RegisterComponent() {
			const char* typeName = typeid(T).name();

			if (m_instance->ComponentExists(typeName)) {
				RDT_CORE_WARN("ComponentManager - Tried to register duplicate component '{}'", typeName);
				return;
			}

			if (m_instance->MaxComponentsReached()) {
				RDT_CORE_FATAL("ComponentManager - Max components reached!", typeName);
				assert(false);
			}

			m_instance->RegisterComponentImpl(typeName, new Component<T>());
		}

		/*
			Returns the component ID for this component if it is registered.
		*/
		template <typename T>
		static ComponentID GetComponentID()
		{
			const char* typeName = typeid(T).name();
			return m_instance->GetComponentIDImpl(typeName);
		}


		/*
			Updates the provided signature to include the component T
		*/
		template<typename T>
		static void UpdateSignature(Signature& signature)
		{
			const char* typeName = typeid(T).name();
			ComponentID cID = m_instance->GetComponentIDImpl(typeName);

			if (cID == NOT_REGISTERED_COMPONENT) {
				return;
			}

			signature.set(cID, true);
		}

		/*
			Returns a non-type pointer this entity's data component
		*/
		static void* GetData(ComponentID cID, Entity entity);

		/*
			Returns the name of the component at the provided ComponentID
		*/
		static const char* GetComponenentName(ComponentID cID);

		friend class EntityManager;
		friend class System;
		friend class core::Realm;

	private:
		template<typename T>
		static void AddToComponent(Entity eID, const T& nData)
		{
			Component<T>* component = GetComponent<T>();

			if (component == nullptr) {
				return;
			}

			component->InsertData(eID, nData);
		}

		template<typename T>
		static void RemoveFromComponent(Entity eID)
		{
			Component<T>* component = GetComponent<T>();

			if (component == nullptr) {
				return;
			}

			component->RemoveData(eID);
		}


		template<typename T>
		static Component<T>* GetComponent()
		{
			const char* typeName = typeid(T).name();
			if (!m_instance->ComponentExists(typeName)) {
				RDT_CORE_WARN("ComponentManager - Tried to use unregistered component '{}'", typeName);
				return nullptr;
			}

			return (Component<T>*)m_instance->GetComponentImpl(typeName);
		}

		/*
			Removes this entity from all components
		*/
		static void OnEntityRemoved(Entity eID);

		bool ComponentExists(const char* typeName);
		bool MaxComponentsReached();
		void RegisterComponentImpl(const char* typeName, IComponentArray* component);
		void* GetComponentImpl(const char* typeName);
		ComponentID GetComponentIDImpl(const char* typeName);

	};
}