#pragma once
#include "scene_api.h"

#define RDT_NULL_SCENE_ID 0

// Forward Declarations
namespace rdt {
	using SceneID = unsigned int;
	using LayerID = unsigned int;
	enum LoopPhase;
}
namespace rdt::scene {
	class SceneManager;
}

namespace rdt::scene {

	class SCENE_API Scene {
	private:
		struct Impl;
		Impl* m_impl;

	protected:
		Scene();
		~Scene();
	public:

		/*
			Returns the global identifier for this scene, registered
			by the SceneManager. If this scene has no registered ID,
			returns 0.
		*/
		SceneID GetID();

		/*
			Returns the registered name for this scene.
		*/
		const char* GetName();

		/*
			Returns true if this is the currently bound scene
		*/
		bool IsBound();

		template<typename T>
		friend SceneID RegisterScene(const char* sceneName);

		friend class SceneManager;

	protected:

		/*
			To implement function that is called before Bind(), allowing
			child scenes to add additional functionality before binding
			a scene.
		*/
		virtual void OnBind() {}

		/*
			To implement function that is called before Release(), allowing
			child scenes to add additional functionality before releasing a
			scene.
		*/
		virtual void OnRelease() {}

		/*
			Attaches a layer to the end of the layer stack.
		*/
		void AttachLayer(const char* layerName);
		
		/*
			Inserts and attaches the layer into the layer stack at
			the given index.
		*/
		void AttachLayer(const char* layerName, size_t insertIndex);

	private:

		static SceneID RegisterSceneImpl(const char* sceneName, Scene* scene);
		void SetSceneID(SceneID nID);
		void SetSceneName(const char* nName);
		void RemoveFromLayerStack(LayerID layer);
		void DetachAll();

		void Bind();
		void Release();

		void ProcessInput(const float deltaTime);
		void WorldUpdate(const float deltaTime);
		void FinalUpdate();
		void RenderUpdate();
	};

	/*
		Registers a new scene to the SceneManager according to the
		child template T.
	*/
	template<typename T>
	SceneID RegisterScene(const char* sceneName)
	{
		Scene* nScene = new T;
		return Scene::RegisterSceneImpl(sceneName, nScene);
	}

	/*
		Returns the registered sceneID for the scene with the
		given name, returns 0 if not found.
	*/
	SCENE_API SceneID GetSceneID(const char* sceneName);

	/*
		Sets/changed the scene to the one with the registered
		name (if it exists)
	*/
	SCENE_API void SetScene(const char* sceneName);

	/*
		Sets/changed the scene to the one with the registered
		ID (if it exists)
	*/
	SCENE_API void SetScene(SceneID sceneID);

	/*
		Calls the update step for the given loop phase, on the
		current scene.
		NOTE: This is called internally by the Radiant Engine (DO NOT USE)
	*/
	SCENE_API void CallUpdate(LoopPhase step, float deltaTime = 0);

	/*
		Destroys the SceneManager and frees all scene resources.
		NOTE: This is called internally by the Radiant Engine (DO NOT USE)
	*/
	SCENE_API void TearDown();
}