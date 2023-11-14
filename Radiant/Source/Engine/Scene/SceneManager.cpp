#include "pch.h"
#include "SceneManager.h"
#include "Messaging/MessageBus.h"
#include "Messaging/MessageTypes.h"
#include "Layer.h"
#include "Logging/Log.h"

#ifdef RDT_USE_DEV_TOOLS
	#include "Editor/Editor.h"
#endif

namespace rdt {
	SceneManager* SceneManager::m_instance = nullptr;

	SceneManager::SceneManager()
	{
		sceneIdCounter = 0;
		layerIdCounter = 0;
		m_current_scene_id = RDT_NULL_SCENE_ID;
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::Initialize()
	{
		Destroy();
		m_instance = new SceneManager;
	}

	void SceneManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	SceneID SceneManager::RegisterScene(const std::string& sceneName, std::shared_ptr<Scene> scene)
	{
		return m_instance->RegisterSceneImpl(sceneName, scene);
	}

	void SceneManager::SetScene(const std::string& sceneName)
	{
		m_instance->SetSceneImpl(sceneName);
	}

	std::shared_ptr<Scene> SceneManager::GetCurrentScene()
	{
		return m_instance->GetCurrentSceneImpl();
	}

	LayerID SceneManager::RegisterLayer(std::shared_ptr<Layer> layer, const std::string& layerName)
	{
		return m_instance->RegisterLayerImpl(layer, layerName);
	}

	std::shared_ptr<Layer> SceneManager::GetLayer(LayerID layer)
	{
		return std::shared_ptr<Layer>();
	}

	SceneID SceneManager::RegisterSceneImpl(const std::string& sceneName, std::shared_ptr<Scene> scene)
	{
		if (SceneExists(sceneName)) {
			RDT_WARN("SceneManager - Scene duplicate found! ({})", sceneName);
			return sceneAliasToId.at(sceneName);
		}

		if (sceneName == "") {
			RDT_WARN("SceneManager - Empty scene names not allowed!");
			return RDT_NULL_SCENE_ID;
		}

		if (scene->GetID() != RDT_NULL_SCENE_ID) {
			RDT_WARN("SceneManager - Tried to register an already-registered layer [id:{}]!", scene->GetID());
			return scene->GetID();
		}

		SceneID nID = NextSceneID();
		scene->SetSceneID(nID);
		scene->SetSceneName(sceneName);
		sceneAliasToId[sceneName] = nID;
		m_scenes[nID] = scene;
		return nID;
	}

	void SceneManager::SetSceneImpl(const std::string& sceneName)
	{
		if (!SceneExists(sceneName)) {
			RDT_CORE_WARN("SceneManager - Could not find scene {}", sceneName);
			return;
		}

		if (SceneExists(m_current_scene_id)) {
			m_scenes.at(m_current_scene_id)->OnRelease();
			m_scenes.at(m_current_scene_id)->Release();
		}

		m_current_scene_id = sceneAliasToId.at(sceneName);
		m_scenes.at(m_current_scene_id)->OnBind();
		m_scenes.at(m_current_scene_id)->Bind();

#ifdef RDT_USE_DEV_TOOLS
		core::Editor::SetCurrentScene(m_scenes.at(m_current_scene_id));
#endif
	}

	std::shared_ptr<Scene> SceneManager::GetCurrentSceneImpl()
	{
		if (!SceneExists(m_current_scene_id)) {
			if (m_current_scene_id == RDT_NULL_SCENE_ID) {
				m_scenes[RDT_NULL_SCENE_ID] = std::make_shared<Scene>();
				RDT_WARN("SceneManager - Using NULL scene");
			}
			else {
				RDT_ERROR("SceneManager - Could not find current scene pointer. Resorting to NULL scene");
				m_current_scene_id = RDT_NULL_SCENE_ID;
				return GetCurrentSceneImpl();
			}
		}
		
		return m_scenes.at(m_current_scene_id);
	}

	LayerID SceneManager::RegisterLayerImpl(std::shared_ptr<Layer> layer, const std::string& layerName)
	{
		if (!layerName.empty()) {
			LayerExists(layerName);
			RDT_WARN("SceneManager - Layer duplicate found ({})!", layerName);
			return layerAliasToId.at(layerName);
		}

		if (layer->GetID() != RDT_NULL_LAYER_ID) {
			RDT_WARN("SceneManager - Tried to register an already-registered layer [id:{}]!", layer->GetID());
			return layer->GetID();
		}

		LayerID nID = NextLayerID();
		layer->SetLayerID(nID);
		layerAliasToId[layerName] = nID;
		m_layers[nID] = layer;
		return nID;
	}

	std::shared_ptr<Layer> SceneManager::GetLayerImpl(LayerID layer)
	{
		if (!LayerExists(layer)) {
			RDT_WARN("SceneManager - Layer [id:{}] not found", layer);
			return nullptr;
		}

		return m_layers.at(layer);
	}

	bool SceneManager::SceneExists(const std::string& sceneName)
	{
		return sceneAliasToId.find(sceneName) != sceneAliasToId.end();
	}

	bool SceneManager::SceneExists(SceneID sceneID)
	{
		return m_scenes.find(sceneID) != m_scenes.end();
	}

	bool SceneManager::LayerExists(const std::string& layerName)
	{
		return layerAliasToId.find(layerName) != layerAliasToId.end();
	}

	bool SceneManager::LayerExists(LayerID layer)
	{
		return m_layers.find(layer) != m_layers.end();
	}

	SceneID SceneManager::NextSceneID()
	{
		return ++sceneIdCounter;
	}

	LayerID SceneManager::NextLayerID()
	{
		return ++layerIdCounter;
	}
}