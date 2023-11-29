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

/***************************************************************
* Forward Declarations
***************************************************************/
namespace rdt {
	using EntityID = unsigned int;
}

namespace rdt {

	// Wrapper class than includes a virtual OnCreate function. This
	// is a to-be-implemented factory method that is used the EntityFactory
	// for the creation of common entities.
	class RDT_ECS_API EntityDefinition {
	private:
		struct Impl;
		Impl* m_impl;

	public:
		EntityDefinition();
		EntityDefinition(const char* entityAlias);
		~EntityDefinition();

		virtual void OnCreate() = 0;

		/*
			Gets the entityID for this registered entity
		*/
		const Entity GetID() const;

	protected:

		/*
			Sets the alias of the entity that is about to be registered.
			Make sure this is called before Register()
		*/
		void SetEntityAlias(const char* alias);

		/*
			Sets the owner of the entity that is about to be registered.
			Make sure this is called before Register()
		*/
		void SetEntityOwner(Layer* layer);
		
		/*
			Utility function that registers a new entity with the
			entity manager and stored the entityID in mID
		*/
		void Register();
	};
}