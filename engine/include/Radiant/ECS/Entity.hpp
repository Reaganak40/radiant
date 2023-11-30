/***************************************************************/
/*  ECS/Entity.hpp                                             */
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
#include <Radiant/ECS/EntityManager.hpp>
#include <string>

/***************************************************************
* Forward Declarations
***************************************************************/
namespace rdt {
	using EntityID = unsigned int;
	class Layer;
}

/***************************************************************
* Client Interface
***************************************************************/
namespace rdt {

	// Wrapper class than includes a virtual OnCreate function. This
	// is a to-be-implemented factory method that is used to abstract
	// away the creation of common entities
	class EntityDefinition {
	private:
		// no dll interface (compile on client-side)
		friend class Layer;

		EntityID m_ID = RDT_NULL_ENTITY_ID;
		std::string entityAlias = "";

	public:
		EntityDefinition();
		~EntityDefinition();

		virtual void OnCreate() = 0;

		inline const EntityID GetID() const { return m_ID; }
		inline const char* GetAlias() const { return entityAlias.c_str(); }

	protected:
/***************************************************************
* Entity Builder Functions
***************************************************************/

		// Sets the alias of the entity that is about to be registered.
		// Call this in the constructor or before OnCreate()
		void SetEntityAlias(const char* alias)
		{
			entityAlias = alias;
		}

		// Adds a component to this entity
		template<typename T>
		void AddComponent(const T& nData = T())
		{
			ecs::EntityManager::AddComponent<T>(GetID(), nData);
		}
	};
}