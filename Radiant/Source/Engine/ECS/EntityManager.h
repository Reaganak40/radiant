#pragma once
#include "Core.h"
#include "ECSTypes.h"
#include "ComponentManager.h"

namespace rdt {
	class RADIANT_API EntityManager {
	private:
		struct Impl;
		Impl* m_impl;
		
		EntityManager();
		~EntityManager();
		static EntityManager* m_instance;

	public:

		/*
			Create a new singleton instance of the EntityManager
		*/
		static void Initialize();

		/*
			Destroy the current singleton instance of the EntityManager
		*/
		static void Destroy();

		/*
			Create a new entity by registering the returned EntityID
		*/
		static Entity RegisterEntity();

		/*
			Removes a registered entity that is referenced by the eID
		*/
		static void RemoveEntity(Entity eID);

		/*
			Sets the component signature for this entity.
		*/
		static void SetSignature(Entity eID, const Signature& signature);

		/*
			Gets the component signature for this entity.
		*/
		static Signature GetSignature(Entity eID);

		/*
			Adds a componenet to this entity, modifying its signature
		*/
		template<typename T>
		static void AddComponent(Entity eID, const T& nData = T())
		{
			ComponentID cID = ComponentManager::GetComponentID<T>();

			if (cID == NOT_REGISTERED_COMPONENT) {
				const char* typeName = typeid(T).name();
				RDT_CORE_WARN("EntityManager - Could not add unregistered component '{}' to entity.", typeName);
				return;
			}

			AddToSignature(eID, cID);
			ComponentManager::AddToComponent<T>(eID, nData);
		}

		template<typename T>
		static void RemoveComponent(Entity eID)
		{
			ComponentID cID = ComponentManager::GetComponentID<T>();

			if (cID == NOT_REGISTERED_COMPONENT) {
				const char* typeName = typeid(T).name();
				RDT_CORE_WARN("EntityManager - Could not remove unregistered component '{}' to entity.", typeName);
				return;
			}

			RemoveFromSignature(eID, cID);
			ComponentManager::RemoveFromComponent<T>(eID);
		}

		/*
			Returns a pointer to this entities component data,
			if this entity has added that component.
		*/
		template<typename T>
		static T* GetComponent(Entity eID)
		{
			if (!EntityExists(eID)) {
				RDT_CORE_WARN("EntityManager - Could get get component from unregistered entity [{}].", eID);
				return nullptr;
			}

			if (ComponentManager::GetComponentID<T>() == NOT_REGISTERED_COMPONENT) {
				const char* typeName = typeid(T).name();
				RDT_CORE_WARN("EntityManager - Could not get unregistered component '{}' from entity.", typeName);
				return nullptr;
			}

			Component<T>* component = ComponentManager::GetComponent<T>();
			if (!component->HasEntity(eID)) {
				const char* typeName = typeid(T).name();
				RDT_CORE_WARN("EntityManager - Entity [{}] is not regisrted to component '{}'.", eID, typeName);
				return nullptr;
			}

			return &component->GetData(eID);
		}

	private:
		/*
			Adds this component to the entity's signature.
		*/
		static void AddToSignature(Entity eID, ComponentID cID);


		/*
			Adds this component to the entity's signature.
		*/
		static void RemoveFromSignature(Entity eID, ComponentID cID);

		/*
			Returns true if the entity is registered
		*/
		static bool EntityExists(Entity eID);
	};
}