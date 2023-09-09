#pragma once
#include "Scene.h"

namespace Radiant {

	class SceneManager {
	private:
		SceneManager();
		~SceneManager();
		static SceneManager* m_instance;

		std::unordered_map<std::string, Scene*> m_scenes;
		std::string m_currentSceneName;
		Scene* m_current_scene;

	public:

		static void Initialize();
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

	private:
		Scene* GetCurrentSceneImpl();
	};
}