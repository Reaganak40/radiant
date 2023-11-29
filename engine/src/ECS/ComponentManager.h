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
#include <Radiant/ECS/ECSTypes.hpp>
#include <Radiant/Logger.hpp>

// Forward Delcarations
namespace rdt::core {
	class Realm;
}

namespace rdt::ecs {

	
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

		/*
			Returns true if the given component name is for a component that is hidden from the client.
		*/
		static bool IsHiddenComponent(const std::string& name);

		/*
			Returns the number of components registered to the manager.
		*/
		static size_t GetComponentCount();

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
			Returns the base pointer of a component that matches the componentID,
			returns nullptr otherwise.
		*/
		static IComponentArray* GetComponentByID(ComponentID cID);

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