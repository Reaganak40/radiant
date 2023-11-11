/*******************************************************************
*	Module:  Scene (core)
*	File:    SceneManager.h
*
*	Author: Reagan Kelley
*
*	The SceneManager is a core singleton that manages the creation
*	and storage of Scenes.
*******************************************************************/
#pragma once

// Required Definitions for Struct/Class Members
#include "Scene.h"

namespace rdt {

	class SceneManager {
	private:
		SceneManager();
		~SceneManager();
		static SceneManager* m_instance;

		std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
		std::string m_currentSceneName;
		std::shared_ptr<Scene> m_current_scene;

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
		static void RegisterScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

		/*
			Sets the current scene by providing its scene name. The associated
			scene will be returned next time the current scene is requested.
		*/
		static void SetScene(const std::string& sceneName);

		/*
			Gets the currently set scene.
		*/
		static std::shared_ptr<Scene> GetCurrentScene();

	private:

		/*
			Unselects the scene, effectively setting the scene pointer
			for current scene to nullptr. This should be used if the
			intention is to close the game.
		*/
		static void UnselectScene() { m_instance->m_currentSceneName = ""; }

		std::shared_ptr<Scene> GetCurrentSceneImpl();
	};
}