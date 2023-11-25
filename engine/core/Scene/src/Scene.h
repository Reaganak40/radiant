#pragma once
#include "scene_api.h"

#define RDT_NULL_SCENE_ID 0
#define RDT_NULL_LAYER_ID 0

// Forward Declarations
namespace rdt {
	using SceneID = unsigned int;
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

	private:

		void SetSceneID(SceneID nID);
		void SetSceneName(const char* nName);
	};

	/*
		Registers a new scene to the SceneManager according to the
		child template T.
	*/
	template<typename T>
	SCENE_API SceneID RegisterScene(const char* sceneName)
	{
		Scene* nScene = new T();
		return RegisterScene(sceneName, nScene);
	}
	
	SceneID RegisterScene(const char* sceneName, Scene* scene);

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
	void CallUpdate(LoopPhase step);
}