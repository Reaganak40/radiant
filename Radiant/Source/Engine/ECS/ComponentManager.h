#pragma once
#include "Logging/Log.h"
#include "Component.h"

using ComponentType = std::uint32_t;
constexpr ComponentType MAX_COMPONENTS = 32;
constexpr ComponentType NOT_REGISTERED_COMPONENT = MAX_COMPONENTS + 1;

using Signature = std::bitset<MAX_COMPONENTS>;

namespace rdt {
	class ComponentManager {
	private:
		ComponentManager();
		~ComponentManager();
		static ComponentManager* m_instance;

		ComponentType m_componentCounter;
		std::unordered_map<const char*, ComponentType> m_componentTypes;
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

			if (m_componentTypes.find(typeName) != m_componentTypes.end()) {
				RDT_CORE_WARN("ComponentManager - Tried to register duplicate component '{}'", typeName);
				return;
			}

			if (m_componentCounter == MAX_COMPONENTS) {
				RDT_CORE_FATAL("ComponentManager - Max components reached!", typeName);
				assert(false);
			}

			m_componentTypes[typeName] = m_componentCounter++;
			m_components[typeName] = new Component<T>();
		}

		template <typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			if (m_componentTypes.find(typeName) == m_componentTypes.end()) {
				RDT_CORE_WARN("ComponentManager - Could not get component type for '{}'", typeName);
				return NOT_REGISTERED_COMPONENT;
			}

			return m_componentTypes.at(typeName);
		}

		template<typename T>
		void AddComponent(EntityID eID, T component)
		{
			Component<T>* component = GetComponent<T>();

			if (component == nullptr) {
				return;
			}

			component->InsertData(eID, T);
		}

		template<typename T>
		void RemoveComponent(EntityID eID)
		{
			Component<T>* component = GetComponent<T>();

			if (component == nullptr) {
				return;
			}

			component->RemoveData(eID);
		}

	private:

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

	};
}