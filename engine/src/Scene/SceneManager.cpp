#include "pch.h"
#include "SceneManager.hpp"
#include "SceneImpl.hpp"
#include "LayerImpl.hpp"
#include <Radiant/Logger.hpp>

rdt::scene::SceneManager* rdt::scene::SceneManager::m_instance = nullptr;

rdt::scene::SceneManager::SceneManager()
{
	sceneIdCounter = 0;
	layerIdCounter = 0;
	m_current_scene_id = RDT_NULL_SCENE_ID;
}

rdt::scene::SceneManager::~SceneManager()
{
	GetCurrentScene()->Release();

	for (auto& [id, scene] : m_scenes) {
		scene->DetachAll();
	}

	for (auto& [id, layer] : m_layers) {
		delete layer;
	}

	for (auto& [id, scene] : m_scenes) {
		delete scene;
	}
}

rdt::scene::SceneManager* rdt::scene::SceneManager::Get()
{
	if (m_instance == nullptr) {
		m_instance = new SceneManager();
	}

	return m_instance;
}

void rdt::scene::SceneManager::Destroy()
{
	if (m_instance != nullptr) {
		delete m_instance;
		m_instance = nullptr;
	}
}

rdt::SceneID rdt::scene::SceneManager::RegisterScene(const char* sceneName, Scene* scene)
{
	if (SceneExists(sceneName)) {
		RDT_CORE_WARN("SceneManager - Tried to registered duplicate scene: {}", sceneName);
		delete scene;
		return GetSceneIDImpl(sceneName);
	}

	if (std::string(sceneName) == "") {
		RDT_CORE_WARN("SceneManager - Cannot use empty scene name");
		delete scene;
		return RDT_NULL_SCENE_ID;
	}


	SceneID nID = NextSceneID();
	scene->GetImpl().SetSceneID(nID);
	scene->GetImpl().SetSceneName(sceneName);
	sceneAliasToId[sceneName] = nID;
	m_scenes[nID] = scene;
	return nID;
}

rdt::SceneID rdt::scene::SceneManager::GetSceneID(const char* sceneName)
{
	if (SceneExists(sceneName)) {
		return sceneAliasToId.at(sceneName);
	}

	return RDT_NULL_SCENE_ID;
}

rdt::Scene* rdt::scene::SceneManager::SetScene(const char* sceneName)
{
	if (!SceneExists(sceneName)) {
		RDT_CORE_WARN("SceneManager - Could not find scene {}", sceneName);
		return;
	}

	if (m_current_scene_id == sceneAliasToId.at(sceneName)) {
		RDT_CORE_WARN("SceneManager - Tried to set Scene '{}' but is already set.", sceneName);
		return;
	}

	if (SceneExists(m_current_scene_id)) {
		m_scenes.at(m_current_scene_id)->Release();
	}

	m_current_scene_id = sceneAliasToId.at(sceneName);
	m_scenes.at(m_current_scene_id)->Bind();
	return m_scenes.at(m_current_scene_id);
}

rdt::LayerID rdt::scene::SceneManager::RegisterLayer(const char* layerName, Layer* nLayer)
{
	if (LayerExists(layerName)) {
		RDT_CORE_WARN("SceneManager - Tried to registered duplicate scene: {}", layerName);
		delete nLayer;
		return GetLayerIDImpl(layerName);
	}

	if (std::string(layerName) == "") {
		RDT_CORE_WARN("SceneManager - Cannot use empty scene name");
		delete nLayer;
		return RDT_NULL_SCENE_ID;
	}

	LayerID nID = NextLayerID();
	nLayer->GetImpl().SetLayerID(nID);
	nLayer->GetImpl().SetName(layerName);
	layerAliasToId[layerName] = nID;
	m_layers[nID] = nLayer;
	return nID;
}

rdt::LayerID rdt::scene::SceneManager::GetLayerID(const char* layerName)
{
	if (LayerExists(layerName)) {
		return layerAliasToId.at(layerName);
	}
	return RDT_NULL_LAYER_ID;
}

rdt::Layer* rdt::scene::SceneManager::AttachLayerToScene(const char* layerName, SceneID scene)
{
	LayerID layerID = GetLayerID(layerName);
	if (layerID == RDT_NULL_LAYER_ID) {
		RDT_CORE_WARN("AttachLayer - Layer '{}' not found!", layerName);
		return nullptr;
	}

	Layer* layer = m_layers.at(layerID);
	SceneID old_scene = layer->GetImpl().GetAttachedSceneID();
	
	if (old_scene != RDT_NULL_SCENE_ID) {
		m_scenes.at(old_scene)->GetImpl().RemoveFromLayerStack(layerID);
		layer->Detach();
	}

	layer->Attach(scene);
	return layer;
}

bool rdt::scene::SceneManager::SceneExists(const std::string& sceneName)
{
	return sceneAliasToId.find(sceneName) != sceneAliasToId.end();
}

bool rdt::scene::SceneManager::SceneExists(SceneID sceneID)
{
	return m_scenes.find(sceneID) != m_scenes.end();
}

bool rdt::scene::SceneManager::LayerExists(const std::string& layerName)
{
	return layerAliasToId.find(layerName) != layerAliasToId.end();

}

bool rdt::scene::SceneManager::LayerExists(LayerID layer)
{
	return m_layers.find(layer) != m_layers.end();
}


rdt::SceneID rdt::scene::SceneManager::NextSceneID()
{
	return ++sceneIdCounter;
}

rdt::LayerID rdt::scene::SceneManager::NextLayerID()
{
	return ++layerIdCounter;
}
