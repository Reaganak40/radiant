/***************************************************************/
/*  ECS/System.hpp                                             */
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
#include <type_traits>
#include <unordered_map>
#include <typeindex>
#include <string>


namespace rdt {
	enum SystemType {
		SystemNAT,			// NOT A TYPE
		SystemProcessInput, // System for OnProcessInput()
		SystemWorldUpdate,  // System for OnWorldUpdate()
		SystemFinalUpdate,	// System for OnFinalUpdate()
		SystemRenderUpdate, // System for OnRender()
	};



	// Systems are used to instantiate unique proecures by the client.
	class System {
		// no dll interface (compile on client-side)
	private:

		std::unordered_map<std::type_index, IComponentArray*> in_use_components;

	protected:

		// Delcares the usage of a component for this system. This should
		// be called in a system's constructor to define the nature of 
		// the system.
		template<typename T>
		static void UseComponent()
		{
			ComponentID cID = ecs::ComponentManager::GetComponentID<T>();

			if (cID == RDT_NULL_COMPONENT_ID) {
				ecs::ComponentManager::RegisterComponent<T>();
				cID = ecs::ComponentManager::GetComponentID<T>();
			}

			const char* typeName = typeid(T).name();
			in_use_components[typeName] = static_cast<IComponentArray>(ecs::ComponentManager::GetComponent(cID));
		}

		// Gets a component used by this system. If the component was not declared to be used
		// will return nullptr.
		template<typename T>
		Component<T>* GetComponent()
		{
			const char* typeName = typeid(T).name();
			if (in_use_components.find(typeName) == in_use_components.end()) {
				return nullptr;
			}
			return static_cast<Component<T>>(in_use_components.at(typeName));
		}
	};
}