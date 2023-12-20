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
* Forward Declarations
***************************************************************/
namespace rdt {
	using SceneID = unsigned int;
	using LayerID = unsigned int;

	class Application;
	class Scene;
	class Layer;
}

/***************************************************************
* Implementation Interface (not intended for client use)
***************************************************************/
namespace rdt::ecs {

	class RDT_ECS_API SystemManager {
	private:

		/***************************************************************
		* EntityManager creation and destruction
		***************************************************************/
		friend class Application;
		static void Initialize();
		static void Destroy();

		friend class Scene;
		friend class Layer;

		// Adds a new system 
		static void RegisterUniversalSystem(SystemID sID, System* nSystem);

		static void RegisterSceneSystem(SceneID sceneID, SystemID systemID, System* nSystem);

		static void RegisterLayerSystem(LayerID lID, SystemID sID, System* nSystem);
	};
}