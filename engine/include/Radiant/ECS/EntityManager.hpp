/***************************************************************/
/*  ECS/EntityManager.hpp                                      */
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
#include <Radiant/ECS/Export.hpp>
#include <Radiant/ECS/ComponentManager.hpp>

/***************************************************************
* Forward Declarations
***************************************************************/
namespace rdt {
	using EntityID = unsigned int;
	class EntityDefinition;
	class Layer;
}

/***************************************************************
* Implementation Interface (not intended for client use)
***************************************************************/
namespace rdt::ecs {

	class RDT_ECS_API EntityManager {
		struct Impl;
		static Impl* m_impl;

/***************************************************************
* EntityManager creation and destruction
***************************************************************/
		friend class Application;
		static void Initialize();
		static void Destroy();

/***************************************************************
* Templates
***************************************************************/
		friend class EntityDefinition;
		friend class Layer;

		// Attaches a new component to an entity, changing its signature
		// and behavior
		template<typename T>
		static void AddComponent(EntityID eID, const T& nData = T())
		{
			ComponentID cID = ComponentManager::GetComponentID(GetComponentType<T>());
			if (cID == RDT_NULL_COMPONENT_ID) {
				ComponentManager::RegisterComponent<T>();
			}

			AddToSignature(eID, cID);

			// should not be null here
			((Component<T>*)ComponentManager::GetComponent(cID))->InsertData(eID, nData);
		}


		// Removes a component from an entity's signature, but caches
		// its data for reuse.
		template<typename T>
		static void RemoveComponent(EntityID eID)
		{

		}

/***************************************************************
* Inteface to implementation
***************************************************************/

		// Create a new entity by registering the returned EntityID. An EnityConfig
		// component will automatically be added to this entity.
		// 
		// \param alias - the name of the entity, when alias string is empty, the EntityManager will provide
		// a name to it.
		static EntityID RegisterEntity(const char* alias = "");

		// Unregisters an entity, removing it from all systems and stops tracking all its components.
		static void RemoveEntity(EntityID entity);
		
		// Updates the entity signature, denoting that it has the indicated component.
		static void AddToSignature(EntityID entity, ComponentID cID);

		// Uses an entity's current signature to subscribe to all registered
		// systems.
		static void SubscribeToSystems();
	};
}
