#pragma once
#include "Scene.h"

namespace rdt {

	class SceneManager {
	private:
		SceneManager();
		~SceneManager();
		static SceneManager* m_instance;

		std::unordered_map<std::string, Scene*> m_scenes;
		std::string m_currentSceneName;
		Scene* m_current_scene;

	public:

		/*
			Creates a new singleton instance of the SceneManager
		*/
		static void Initialize();

		/*
			Destroys the SceneManger singelton instance.
		*/
		static void Destroy();

		/*
			Adds a scene to the manager's map of name-associated scenes.
		*/
		static void RegisterScene(const std::string& sceneName, Scene* scene);

		/*
			Sets the current scene by providing its scene name. The associated
			scene will be returned next time the current scene is requested.
		*/
		static void SetScene(const std::string& sceneName);

		/*
			Gets the currently set scene.
		*/
		static Scene* GetCurrentScene() { return m_instance->GetCurrentSceneImpl(); }

		friend class Scene;
	private:

		/*
			Returns a scene even if it is not active. Used by other
			scenes for communication.
		*/
		static Scene* GetAnyScene(const std::string& sceneName);

		/*
			Unselects the scene, effectively setting the scene pointer
			for current scene to nullptr. This should be used if the
			intention is to close the game.
		*/
		static void UnselectScene() { m_instance->m_currentSceneName = ""; }

		Scene* GetCurrentSceneImpl();
	};
}