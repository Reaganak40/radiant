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

#define RDT_NULL_SCENE_ID 0
#define RDT_NULL_LAYER_ID 0

namespace rdt {
	using SceneID = unsigned int;
	using LayerID = unsigned int;

	class SceneManager {
	private:
		SceneManager();
		~SceneManager();
		static SceneManager* m_instance;
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
		static SceneID RegisterScene(const std::string& sceneName, std::shared_ptr<Scene> scene);

		/*
			Sets the current scene by providing its scene name. The associated
			scene will be returned next time the current scene is requested.
		*/
		static void SetScene(const std::string& sceneName);

		/*
			Gets the currently set scene.
		*/
		static std::shared_ptr<Scene> GetCurrentScene();
		
		/*
			Unselects the scene, effectively setting the scene pointer
			for current scene to nullptr. This should be used if the
			intention is to close the game.
		*/
		static void UnselectScene();

		/*
			Provided a layerID to the given layer, which allows it to be identified
			in debugging, but also so it can be exchanged between scenes. May
			provide an alias to the layer for easier global access.
		*/
		static LayerID RegisterLayer(std::shared_ptr<Layer> layer, const std::string& layerName = "");

		/*
			Gets a previously registered layer from the scene manager.
		*/
		static std::shared_ptr<Layer> GetLayer(LayerID layer);

	private:
		SceneID RegisterSceneImpl(const std::string& sceneName, std::shared_ptr<Scene> scene);
		void SetSceneImpl(const std::string& sceneName);
		std::shared_ptr<Scene> GetCurrentSceneImpl();
		LayerID RegisterLayerImpl(std::shared_ptr<Layer> layer, const std::string& layerName = "");
		std::shared_ptr<Layer> GetLayerImpl(LayerID layer);

		std::unordered_map<std::string, SceneID> sceneAliasToId;
		std::unordered_map<SceneID, std::shared_ptr<Scene>> m_scenes;
		SceneID m_current_scene_id;

		std::unordered_map<std::string, LayerID> layerAliasToId;
		std::unordered_map<LayerID, std::shared_ptr<Layer>> m_layers;

		SceneID sceneIdCounter;
		LayerID layerIdCounter;

		bool SceneExists(const std::string& sceneName);
		bool SceneExists(SceneID sceneID);

		bool LayerExists(const std::string& layerName);
		bool LayerExists(LayerID layer);

		/*
			Gets the next available sceneID
		*/
		SceneID NextSceneID();

		/*
			Gets the next available layerID
		*/
		LayerID NextLayerID();
	};
}