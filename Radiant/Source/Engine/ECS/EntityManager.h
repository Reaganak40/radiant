/*******************************************************************
*	Module:  ECS
*	File:    EntityManager.h
*
*	Author: Reagan Kelley
*
*   The file contains the EntityManager which creates new entities,
*	manages there signatures, and lifetimes.
*******************************************************************/
#pragma once
#include "Core.h"
#include "ECSTypes.h"
#include "ComponentManager.h"

#define RDT_NULL_ENTITY_ID 0

#ifdef RDT_USE_DEV_TOOLS
	#define RDT_NUM_HIDDEN_COMPONENTS 2
#else
	#define RDT_NUM_HIDDEN_COMPONENTS 1
#endif

namespace rdt {
	class Layer;

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
			Create a new entity by registering the returned EntityID. An EnityConfig
			component will automatically be added to this entity.

			alias - the name of the entity, when alias string is empty, the EntityManager will provide
			a name to it.

			owner - a pointer to the layer that tracks this entity, nullptr indicates this entity
			has no owner.

		*/
		static Entity RegisterEntity(const std::string& alias = "", Layer* owner = nullptr);

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
			Returns a constant reference to the registered entities.
		*/
		static const std::unordered_map<Entity, Signature>& GetEntityMap();


		template<typename T>
		static bool HasComponent(Entity eID)
		{
			ComponentID cID = ComponentManager::GetComponentID<T>();

			if (cID == NOT_REGISTERED_COMPONENT) {
				const char* typeName = typeid(T).name();
				RDT_CORE_WARN("EntityManager - Could not check unregistered component '{}' for entity.", typeName);
				return false;
			}

			if (!EntityExists(eID)) {
				RDT_CORE_WARN("EntityManager - Could not check component for entity unregisrted entity [{}].", eID);
				return false;
			}

			return m_instance->HasComponentImpl(eID, cID);
		}

		/*
			Adds a componenet to this entity, modifying its signature,
			stores the component data in the component manager.
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

		/*
			Removes a component from this entity, modifying its signature,
			and removing its component data from the component manager.
		*/
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

		/*
			Gets the alias of the provided entity.
		*/
		static const char* GetEntityAlias(Entity entity);

		/*
			Returns a pointer to the layer that owns this entity,
			or nullptr if it is not owned by anyone.
		*/
		static Layer* GetEntityOwner(Entity entity);

		/*
			Maintains the entity signature but disables the use
			of the entity's component data.
		*/
		static void DisableComponent(Entity eID, ComponentID cID);

		/*
			Reintroduced this component, and restoring data access.
		*/
		static void EnableComponent(Entity eID, ComponentID cID);

		/*
			Returns true if the given component is in use.
		*/
		static bool IsComponentEnabled(Entity eID, ComponentID cID);

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


		bool HasComponentImpl(Entity eID, ComponentID cID);
	};
}