/***************************************************************/
/*  (Impl) ECS/EntityManager.hpp                               */
/* *************************************************************/
/*                 This file is a part of:                     */
/*                -- RADIANT GAME ENGINE --                    */
/*         https://github.com/Reaganak40/radiant               */
/***************************************************************/
/*            Copyright(c) 2023 Reagan Kelley                  */
/*                                                             */
/*  Permission  is  hereby  granted, free  of charge, to  any  */
/*  person obtaining a copy of this  software and  associated  */
/*  documentation  files(the  "Software"), to  deal  in   the  */
/*  Software without restriction,including without limitation  */
/*  the   rights   to  use,  copy,  modify,  merge,  publish,  */
/*  distribute,  sublicense,  and  /or  sell  copies  of  the  */
/*  Software,  and to permit persons to whom the  Software is  */
/*  furnished to do so, subject to the following conditions:   */
/*                                                             */
/*  The  above  copyright  notice  and this permission notice  */
/*  shall  be  included in all copies or substantial portions  */
/*  of the Software.                                           */
/*                                                             */
/*  THE  SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF  */
/*  ANY KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT LIMITED  */
/*  TO THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  */
/*  PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT  SHALL  */
/*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  */
/*  DAMAGES OR OTHER  LIABILITY,  WHETHER  IN  AN  ACTION  OF  */
/*  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT  OF  OR IN  */
/*  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  */
/*  IN THE SOFTWARE.                                           */
/***************************************************************/

#pragma once

/***************************************************************
* Headers
***************************************************************/
#include "ComponentManager.h"

/***************************************************************
* Forward Declarations
***************************************************************/

#define RDT_NULL_ENTITY_ID 0

namespace rdt::ecs {

	class EntityManager {
	private:
		EntityManager();
		~EntityManager();
		static EntityManager* m_instance;

		// EntityID management
		EntityID m_idCounter = RDT_NULL_ENTITY_ID;
		std::vector<EntityID> m_idCache;

		// Entity Signature
		std::vector<Signature> m_signatures;
	public:

		static EntityManager& Get();
		static void Destroy();

		// Create a new entity by registering the returned EntityID. An EnityConfig
		// component will automatically be added to this entity.
		// 
		// \param alias - the name of the entity, when alias string is empty, the EntityManager will provide
		// a name to it.
		// 
		// \param owner - a pointer to the layer that tracks this entity, nullptr indicates this entity
		// has no owner.
		EntityID RegisterEntity(const std::string& alias = "");
		void RemoveEntity(EntityID entity);
		void SetSignature(EntityID entity, const Signature& signature);
		Signature GetSignature(EntityID eID);

		template<typename T>
		static bool HasComponent(EntityID eID)
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
		static void AddComponent(EntityID eID, const T& nData = T())
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
		static void RemoveComponent(EntityID eID)
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
		static T* GetComponent(EntityID eID)
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
		static const char* GetEntityAlias(EntityID entity);

		/*
			Returns a pointer to the layer that owns this entity,
			or nullptr if it is not owned by anyone.
		*/
		static Layer* GetEntityOwner(EntityID entity);

		/*
			Maintains the entity signature but disables the use
			of the entity's component data.
		*/
		static void DisableComponent(EntityID eID, ComponentID cID);

		/*
			Reintroduced this component, and restoring data access.
		*/
		static void EnableComponent(EntityID eID, ComponentID cID);

		/*
			Returns true if the given component is in use.
		*/
		static bool IsComponentEnabled(EntityID eID, ComponentID cID);

	private:
		/*
			Adds this component to the entity's signature.
		*/
		static void AddToSignature(EntityID eID, ComponentID cID);


		/*
			Adds this component to the entity's signature.
		*/
		static void RemoveFromSignature(EntityID eID, ComponentID cID);

		/*
			Returns true if the entity is registered
		*/
		static bool EntityExists(EntityID eID);


		bool HasComponentImpl(EntityID eID, ComponentID cID);
	};
}