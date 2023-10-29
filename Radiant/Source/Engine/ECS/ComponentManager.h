#pragma once
#include "ECSTypes.h"
#include "Logging/Log.h"

namespace rdt {

	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;

		virtual void RemoveData(EntityID eID) = 0;
	};

	template <typename T>
	class Component : public IComponentArray {
	private:

		std::unordered_map<EntityID, size_t> m_entity_map;

		std::vector<EntityID> m_entities;
		std::vector<T> m_data;
	public:

		/*
			Adds data to the component array while tracking the associated
			entity.
		*/
		void InsertData(EntityID eID, T data) {
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
		void RemoveData(EntityID eID) override final {
			if (m_entity_map.find(eID) == m_entity_map.end()) {
				RDT_CORE_WARN("Component - Could not remove data, entity not found [eID: {}]", eID);
				return;
			}

			size_t index = m_entity_map.at(eID);
			m_data.erase(m_data.begin() + index);
			m_entities.erase(m_entities.begin() + index);
			m_entity_map.erase(eID);

			// Update the data index location of all entities using this component
			for (int i = index; i < m_entities.size(); i++) {
				m_entity_map.at(m_entities[i]) = i;
			}
		}

		T& GetData(EntityID eID) {
			return m_data[m_entity_map.at(eID)];
		}
	};
	// ====================================================================

	class ComponentManager {
	private:
		ComponentManager();
		~ComponentManager();
		static ComponentManager* m_instance;

		ComponentID m_componentCounter;
		std::unordered_map<const char*, ComponentID> m_componentTypes;
		std::unordered_map<const char*, IComponentArray*> m_components;
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

			if (m_instance->m_componentTypes.find(typeName) != m_instance->m_componentTypes.end()) {
				RDT_CORE_WARN("ComponentManager - Tried to register duplicate component '{}'", typeName);
				return;
			}

			if (m_instance->m_componentCounter == MAX_COMPONENTS) {
				RDT_CORE_FATAL("ComponentManager - Max components reached!", typeName);
				assert(false);
			}

			m_instance->m_componentTypes[typeName] = m_instance->m_componentCounter++;
			m_instance->m_components[typeName] = new Component<T>();
		}

		/*
			Returns the component ID for this component if it is registered.
		*/
		template <typename T>
		ComponentID GetComponentID()
		{
			const char* typeName = typeid(T).name();

			if (m_componentTypes.find(typeName) == m_componentTypes.end()) {
				RDT_CORE_WARN("ComponentManager - Could not get component type for '{}'", typeName);
				return NOT_REGISTERED_COMPONENT;
			}

			return m_componentTypes.at(typeName);
		}

		friend class EntityManager;
		friend class System;
	private:
		template<typename T>
		void AddToComponent(EntityID eID, T nData)
		{
			Component<T>* component = GetComponent<T>();

			if (component == nullptr) {
				return;
			}

			component->InsertData(eID, nData);
		}

		template<typename T>
		void RemoveFromComponent(EntityID eID)
		{
			Component<T>* component = GetComponent<T>();

			if (component == nullptr) {
				return;
			}

			component->RemoveData(eID);
		}


		template<typename T>
		Component<T>* GetComponent()
		{
			const char* typeName = typeid(T).name();
			if (m_componentTypes.find(typeName) == m_componentTypes.end()) {
				RDT_CORE_WARN("ComponentManager - Tried to use unregistered component '{}'", typeName);
				return nullptr;
			}

			return (Component<T>*)m_components.at(typeName);
		}

		/*
			Removes this entity from all components
		*/
		static void OnEntityRemoved(EntityID eID);

	};
}