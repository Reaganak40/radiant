#include "pch.h"
#include "SceneManagerImpl.hpp"
#include "SceneImpl.hpp"
#include "LayerImpl.hpp"
#include <Radiant/Logger.hpp>

rdt::scene::SceneManagerImpl* rdt::scene::SceneManagerImpl::m_instance = nullptr;

rdt::scene::SceneManagerImpl::SceneManagerImpl()
{
	sceneIdCounter = 0;
	layerIdCounter = 0;
	m_current_scene_id = RDT_NULL_SCENE_ID;
}

rdt::scene::SceneManagerImpl::~SceneManagerImpl()
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

rdt::scene::SceneManagerImpl* rdt::scene::SceneManagerImpl::Get()
{
	if (m_instance == nullptr) {
		m_instance = new SceneManager();
	}

	return m_instance;
}

void rdt::scene::SceneManagerImpl::Destroy()
{
	if (m_instance != nullptr) {
		delete m_instance;
		m_instance = nullptr;
	}
}

rdt::SceneID rdt::scene::SceneManagerImpl::RegisterScene(const char* sceneName, Scene* scene)
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

rdt::SceneID rdt::scene::SceneManagerImpl::GetSceneID(const char* sceneName)
{
	if (SceneExists(sceneName)) {
		return sceneAliasToId.at(sceneName);
	}

	return RDT_NULL_SCENE_ID;
}

rdt::Scene* rdt::scene::SceneManagerImpl::SetScene(const char* sceneName)
{
	if (!SceneExists(sceneName)) {
		RDT_CORE_WARN("SceneManager - Could not find scene {}", sceneName);
		return nullptr;
	}

	if (m_current_scene_id == sceneAliasToId.at(sceneName)) {
		RDT_CORE_WARN("SceneManager - Tried to set Scene '{}' but is already set.", sceneName);
		return nullptr;
	}

	return SetScene(sceneAliasToId.at(sceneName));
}

rdt::Scene* rdt::scene::SceneManagerImpl::GetCurrentScene()
{
	if (m_current_scene_id == RDT_NULL_SCENE_ID) {
		return nullptr;
	}

	return m_scenes.at(m_current_scene_id);
}

rdt::LayerID rdt::scene::SceneManagerImpl::RegisterLayer(const char* layerName, Layer* nLayer)
{
	if (LayerExists(layerName)) {
		RDT_CORE_WARN("SceneManager - Tried to registered duplicate scene: {}", layerName);
		delete nLayer;
		return GetLayerID(layerName);
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

rdt::LayerID rdt::scene::SceneManagerImpl::GetLayerID(const char* layerName)
{
	if (LayerExists(layerName)) {
		return layerAliasToId.at(layerName);
	}
	return RDT_NULL_LAYER_ID;
}

rdt::Layer* rdt::scene::SceneManagerImpl::AttachLayerToScene(const char* layerName, SceneID scene)
{
	LayerID layerID = GetLayerID(layerName);
	if (layerID == RDT_NULL_LAYER_ID) {
		RDT_CORE_WARN("SceneManager - Could not attach layer '{}', it does not exist", layerName);
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

void rdt::scene::SceneManagerImpl::RequestToChangeScene(const char* sceneName)
{
	if (SceneExists(sceneName)) {
		RDT_CORE_WARN("Cannot change to scene '{}', is does not exist.", sceneName);
		return;
	}

	m_scene_request = sceneAliasToId.at(sceneName);
}

int rdt::scene::SceneManagerImpl::OnEndFrame()
{
	if (m_scene_request != RDT_NULL_SCENE_ID) {
		SetScene(m_scene_request);
		m_scene_request = RDT_NULL_SCENE_ID;
		return 1;
	}

	return 0;
}

bool rdt::scene::SceneManagerImpl::SceneExists(const std::string& sceneName)
{
	return sceneAliasToId.find(sceneName) != sceneAliasToId.end();
}

bool rdt::scene::SceneManagerImpl::SceneExists(SceneID sceneID)
{
	return m_scenes.find(sceneID) != m_scenes.end();
}

bool rdt::scene::SceneManagerImpl::LayerExists(const std::string& layerName)
{
	return layerAliasToId.find(layerName) != layerAliasToId.end();

}

bool rdt::scene::SceneManagerImpl::LayerExists(LayerID layer)
{
	return m_layers.find(layer) != m_layers.end();
}

rdt::Scene* rdt::scene::SceneManagerImpl::SetScene(SceneID scene)
{
	if (SceneExists(m_current_scene_id)) {
		m_scenes.at(m_current_scene_id)->Release();
	}

	m_current_scene_id = scene;
	m_scenes.at(m_current_scene_id)->Bind();
	return m_scenes.at(m_current_scene_id);
}


rdt::SceneID rdt::scene::SceneManagerImpl::NextSceneID()
{
	return ++sceneIdCounter;
}

rdt::LayerID rdt::scene::SceneManagerImpl::NextLayerID()
{
	return ++layerIdCounter;
}
