/***************************************************************/
/*  Scene/Layer.hpp                                            */
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

// 
#include <Radiant/ECS/Entity.hpp>

// Forward Declarations
namespace rdt {
	using LayerID  = unsigned int;
	using SceneID  = unsigned int;
}
namespace rdt::scene {
	struct LayerImpl;
	struct SceneImpl;
	class SceneManagerImpl;
}

#define RDT_NULL_LAYER_ID 0

namespace rdt {
	class RDT_SCENE_API Layer {
	private:
		scene::LayerImpl* m_impl;

	protected:
		Layer();
		~Layer();
	public:
		
		// Gets the ID for this layer. Returns 0 if this layer
		// has no registered ID.
		LayerID GetID();

		// Gets the registered name for this Layer
		const char* GetName();

		// Returns true if this layer is attached to a scene.
		bool IsAttached();

		// Returns true if this layer has been bounded by the scene
		bool IsBound();

	protected:

		// To implement function, called after this layer is assigned to a scene.
		virtual void OnAttach() {}

		// To implement function, called before this layer is unassigned from a scene.
		virtual void OnDetach() {}

		// To implement function, called when the owner of the scene becomes the active
		// scene.
		virtual void OnBind() {}

		// To implement function, called when the host layer scene is released
		// (no longer the active scene)
		virtual void OnRelease() {}

		// To implement function, called right before the base class
		// calls ProcessInput.
		virtual void OnProcessInput(const float deltaTime) {}

		// To implement function, called right before the base class
		// calls UpdateWorld.
		virtual void OnUpdateWorld(const float deltaTime) {}

		// To implement function, called right before the base class
		// calls FinalUpdate.
		virtual void OnFinalUpdate() {}

		// To implement function, called right before the base class
		// calls RenderUpdate.
		virtual void OnRenderUpdate() {}

		// Calls to change to a new scene, which will happen at the end
		// of the current game loop.
		void ChangeScene(const char* nScene);


		// Adds a new entity to the layer using the given entity definition and arguments. Returns the
		// new entity's global unique identifier. If the creation of the entity failed, returns 0.
		template <class EntityDef, typename... EntityArgs>
		EntityID AddEntity(EntityArgs... args);

	private:

		EntityID AddEntityImpl(EntityDefinition& entityDef);


		template<typename T>
		friend LayerID RegisterLayer(const char* layerName);
		static LayerID RegisterLayerImpl(const char* layerName, Layer* nLayer);

		friend class scene::SceneManagerImpl;
		friend struct scene::SceneImpl;
		
		scene::LayerImpl& GetImpl();
		void Attach(SceneID sID);
		void Detach();
		void Bind();
		void Release();
		void ProcessInput(const float deltaTime);
		void WorldUpdate(const float deltaTime);
		void FinalUpdate();
		void RenderUpdate();
	};

	template<typename T>
	LayerID RegisterLayer(const char* layerName)
	{
		Layer* nLayer = new T();
		return Layer::RegisterLayerImpl(layerName, nLayer);
	}
	template<class EntityDef, typename ...EntityArgs>
	inline EntityID Layer::AddEntity(EntityArgs ...args)
	{
		EntityDef nEntity(args..);
		return AddEntityImpl(static_cast<EntityDefinition>(nEntity));
	}
}