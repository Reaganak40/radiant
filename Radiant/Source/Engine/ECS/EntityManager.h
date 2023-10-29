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
		static EntityID RegisterEntity();

		/*
			Removes a registered entity that is referenced by the eID
		*/
		static void RemoveEntity(EntityID eID);

		/*
			Sets the component signature for this entity.
		*/
		static void SetSignature(EntityID eID, const Signature& signature);

		/*
			Gets the component signature for this entity.
		*/
		static Signature GetSignature(EntityID eID);

		/*
			Adds a componenet to this entity, modifying its signature
		*/
		template<typename T>
		void AddComponent(EntityID eID, T nData = T())
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
		void RemoveComponent(EntityID eID)
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

	private:
		/*
			Adds this component to the entity's signature.
		*/
		static void AddToSignature(EntityID eID, ComponentID cID);


		/*
			Adds this component to the entity's signature.
		*/
		static void RemoveFromSignature(EntityID eID, ComponentID cID);
	};
}