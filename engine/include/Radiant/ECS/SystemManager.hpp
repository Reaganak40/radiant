/***************************************************************/
/*  ECS/SystemManager.hpp                                      */
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
#include <Radiant/ECS/System.hpp>

/***************************************************************
* Implementation Interface (not intended for client use)
***************************************************************/
namespace rdt::ecs {

	class RDT_ECS_API SystemManager {
	private:
		template<typename T>
		friend void ::rdt::RegisterSystem();

/***************************************************************
* EntityManager creation and destruction
***************************************************************/
		friend class Application;
		static void Initialize();
		static void Destroy();


		static void RegisterSystem(const char* name, System* nSystem);
	};	
}

/***************************************************************
* Client Interface
***************************************************************/
namespace rdt {
	// Adds a system/procedure to the ECS instance. This system will be enabled by default.
	// Systems can be registered from anywhere, but it is best practice to do it in scenes
	// and child applications.
	template<typename T>
	void RegisterSystem()
	{
		const char* typeName = typeid(T).name();
		T* nSystem = new T;
		static_assert(std::is_base_of<System, T>(nSystem));
		ecs::SystemManager::RegisterSystem(typeName, static_cast<System>(nSystem));
	}
}