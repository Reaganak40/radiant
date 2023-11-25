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

// Forward Declarations
namespace rdt {
	using SceneID = unsigned int;
	using LayerID = unsigned int;
}

// Required Definitions for Struct/Class Members
#include "Scene.h"

namespace rdt::scene {

	class SceneManager {
	private:
		SceneManager();
		~SceneManager();
		static SceneManager* m_instance;

		static SceneManager* Get();
	public:

		/*
			Adds a scene to the manager's map of name-associated scenes.
		*/
		static SceneID RegisterScene(const char* sceneName, Scene* scene);

		static SceneID GetSceneID(const char* sceneName);

		/*
			Sets the current scene by providing its scene name. The associated
			scene will be returned next time the current scene is requested.
		*/
		static void SetScene(const char* sceneName);


	private:
		SceneID RegisterSceneImpl(const char* sceneName, Scene* scene);
		SceneID GetSceneIDImpl(const char* sceneName);


		std::unordered_map<std::string, SceneID> sceneAliasToId;
		std::unordered_map<SceneID, Scene*> m_scenes;
		SceneID m_current_scene_id;

		std::unordered_map<std::string, LayerID> layerAliasToId;
		std::unordered_map<LayerID, Scene*> m_layers;

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