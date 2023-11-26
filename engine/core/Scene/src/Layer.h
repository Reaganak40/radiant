#pragma once
#include "scene_api.h"

// Forward Declarations
namespace rdt {
	using LayerID = unsigned int;
	using SceneID = unsigned int;
}
namespace rdt::scene {
	class SceneManager;
}

#define RDT_NULL_LAYER_ID 0

namespace rdt::scene {
	class SCENE_API Layer {
	private:
		struct Impl;
		Impl* m_impl;

	protected:
		Layer();
		~Layer();
	public:
		/*
			Gets the ID for this layer. Returns 0 if this layer
			has no registered ID.
		*/
		LayerID GetID();

		/*
			Gets the registered name for this Layer
		*/
		const char* GetName();

		/*
			Returns true if this layer is attached to a scene.
		*/
		bool IsAttached();

		/*
			Returns true if this layer has been bounded by the scene
		*/
		bool IsBound();

		template<typename T>
		friend LayerID RegisterLayer(const char* layerName);
		friend class SceneManager;
		friend class Scene;

	protected:

		/*
			To implement function, called after this layer is assigned to a scene.
		*/
		virtual void OnAttach() {}

		/*
			To implement function, called before this layer is unassigned from a scene.
		*/
		virtual void OnDetach() {}

		/*
			To implement function, called when the host layer scene is binded (making it
			the active scene).
		*/
		virtual void OnBind() {}

		/*
			To implement function, called when the host layer scene is released
			(no longer the active scene)
		*/
		virtual void OnRelease() {}

		/*
			To implement function, called right before the base class
			calls ProcessInput.
		*/
		virtual void OnProcessInput(const float deltaTime) {}

		/*
			To implement function, called right before the base class
			calls UpdateWorld.
		*/
		virtual void OnUpdateWorld(const float deltaTime) {}

		/*
			To implement function, called right before the base class
			calls FinalUpdate.
		*/
		virtual void OnFinalUpdate() {}

		/*
			To implement function, called right before the base class
			calls RenderUpdate.
		*/
		virtual void OnRenderUpdate() {}

	private:
		static LayerID RegisterLayerImpl(const char* layerName, Layer* nLayer);
		void SetLayerID(LayerID nID);
		void SetName(const char* name);

		SceneID GetAttachedSceneID();

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
}