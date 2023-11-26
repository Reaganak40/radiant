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
#include "Layer.h"

namespace rdt::scene {

	class SceneManager {
	private:
		SceneManager();
		~SceneManager();
		static SceneManager* m_instance;

		static SceneManager* Get();
	public:
		static void TearDown();

		static SceneID RegisterScene(const char* sceneName, Scene* scene);
		static SceneID GetSceneID(const char* sceneName);
		static void SetScene(const char* sceneName);

		static LayerID RegisterLayer(const char* layerName, Layer* nLayer);
		static LayerID GetLayerID(const char* layerName);

		static Layer* AttachLayerToScene(const char* layerName, SceneID scene);

		static void CallUpdate(LoopPhase step, float deltaTime);
	private:
		SceneID RegisterSceneImpl(const char* sceneName, Scene* scene);
		SceneID GetSceneIDImpl(const char* sceneName);
		void SetSceneImpl(const char* sceneName);
		
		LayerID RegisterLayerImpl(const char* layerName, Layer* nLayer);
		LayerID GetLayerIDImpl(const char* layerName);
		Layer* AttachLayerToSceneImpl(LayerID layer, SceneID scene);

		std::unordered_map<std::string, SceneID> sceneAliasToId;
		std::unordered_map<SceneID, Scene*> m_scenes;
		SceneID m_current_scene_id;

		std::unordered_map<std::string, LayerID> layerAliasToId;
		std::unordered_map<LayerID, Layer*> m_layers;

		SceneID sceneIdCounter;
		LayerID layerIdCounter;

		bool SceneExists(const std::string& sceneName);
		bool SceneExists(SceneID sceneID);

		bool LayerExists(const std::string& layerName);
		bool LayerExists(LayerID layer);
		
		Scene* GetCurrentScene();

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