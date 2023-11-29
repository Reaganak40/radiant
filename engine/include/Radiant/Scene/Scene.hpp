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
	class SceneManagerImpl;
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
		virtual void OnBind() = 0;

		// To implement function that is called before Release(), allowing
		// child scenes to add additional functionality before releasing a
		// scene.
		virtual void OnRelease() = 0;

		// Attaches a layer to the end of the layer stack.
		// Layers can only have one owner. If the layer is attached to another
		// scene, it will be detached and reattached to this scene instead.
		void AttachLayer(const char* layerName);
		
		// Inserts and attaches the layer into the layer stack at
		// the given index.
		// Layers can only have one owner. If the layer is attached to another
		// scene, it will be detached and reattached to this scene instead.
		void AttachLayer(const char* layerName, size_t insertIndex);

		// Calls to change to a new scene, which will happen at the end
		// of the current game loop.
		void ChangeScene(const char* nScene);

/***************************************************************
* Layer Binding Functions
***************************************************************/
		// Binds a single layer, that matches the name in the scene's layer stack.
		// NOTE: Layers can only be binded when the scene is bound. Only call this
		// function during OnBind() or while bounded.
		void BindLayer(const char* layerName);

		// Binds a single layer, the one that is at the given index on the scene's layer stack
		// NOTE: Layers can only be binded when the scene is bound. Only call this
		// function during OnBind() or while bounded.
		void BindLayer(size_t layerIndex);

		// Binds all the layers in this scene's layer stack
		// NOTE: Layers can only be binded when the scene is bound. Only call this
		// function during OnBind() or while bounded.
		void BindAllLayers();

/***************************************************************
* Layer Binding Functions
***************************************************************/
		// Binds a single layer, that matches the name in the scene's layer stack.
		// NOTE: Layers are released automatically when the scene is released. Call
		// this function when the scene is bound and the layer no longer serves its
		// use, but the other layers should remain bound.
		void ReleaseLayer(const char* layerName);

		// Binds a single layer, the one that is at the given index on the scene's layer stack
		// NOTE: Layers are released automatically when the scene is released. Call
		// this function when the scene is bound and the layer no longer serves its
		// use, but the other layers should remain bound.
		void ReleaseLayer(size_t layerIndex);

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
		friend class scene::SceneManagerImpl;
		scene::SceneImpl& GetImpl();

/***************************************************************
* Used by Application internals
***************************************************************/
		friend class Application;
		void ProcessInput(const float deltaTime);
		void WorldUpdate(const float deltaTime);
		void FinalUpdate();
		void RenderUpdate();
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