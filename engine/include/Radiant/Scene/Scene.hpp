/***************************************************************/
/*  Scene/Scene.hpp                                            */
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
#include <Radiant/Scene/Export.hpp>

#define RDT_NULL_SCENE_ID 0

// Forward Declarations
namespace rdt {
	using SceneID = unsigned int;
	using LayerID = unsigned int;
	enum LoopPhase;
}
namespace rdt::scene {
	struct SceneImpl;
	class SceneManager;
}

namespace rdt {

	class RDT_SCENE_API Scene {
	private:
		scene::SceneImpl* m_impl;

	protected:
		Scene();
		~Scene();
	public:

		//Returns the global identifier for this scene, registered
		//by the SceneManager. If this scene has no registered ID,
		//returns 0.
		SceneID GetID();

		// Returns the registered name for this scene.
		const char* GetName();

		// Returns true if this is the currently bound scene
		bool IsBound();

	protected:

		// To implement function that is called before Bind(), allowing
		// child scenes to add additional functionality before binding
		// a scene.
		virtual void OnBind() {}

		// To implement function that is called before Release(), allowing
		// child scenes to add additional functionality before releasing a
		// scene.
		virtual void OnRelease() {}

		// Attaches a layer to the end of the layer stack.
		void AttachLayer(const char* layerName);
		
		// Inserts and attaches the layer into the layer stack at
		// the given index.
		void AttachLayer(const char* layerName, size_t insertIndex);

		// Calls to change to a new scene, which will happen at the end
		// of the current game loop.
		void ChangeScene(const char* nScene);

	private:
		void Bind();
		void Release();

/***************************************************************
* Factory method used by client
***************************************************************/
		template<typename T>
		friend SceneID RegisterScene(const char* sceneName);
		static SceneID RegisterSceneImpl(const char* sceneName, Scene* scene);
		
/***************************************************************
* Used by Scene internals
***************************************************************/
		friend class scene::SceneManager;
		scene::SceneImpl& GetImpl();

/***************************************************************
* Used by Application internals
***************************************************************/
		friend class Application;
		void ProcessInput(const float deltaTime);
		void WorldUpdate(const float deltaTime);
		void FinalUpdate();
		void RenderUpdate();
		static void SetCurrentScene(const char* sceneName);
		static Scene& GetCurrentScene();
		static void Destroy();
	};

	// Registers a new scene according to the child template T.
	// Scene names must be unique.
	template<typename T>
	SceneID RegisterScene(const char* sceneName)
	{
		Scene* nScene = new T;
		return Scene::RegisterSceneImpl(sceneName, nScene);
	}
}