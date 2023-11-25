#include "pch.h"
#include "SceneManager.h"

#include <Radiant/Logger.h>

rdt::scene::SceneManager* rdt::scene::SceneManager::m_instance = nullptr;

rdt::scene::SceneManager::SceneManager()
{
	sceneIdCounter = 0;
	layerIdCounter = 0;
	m_current_scene_id = RDT_NULL_SCENE_ID;
}

rdt::scene::SceneManager::~SceneManager()
{
}

rdt::scene::SceneManager* rdt::scene::SceneManager::Get()
{
	if (m_instance == nullptr) {
		m_instance = new SceneManager();
	}

	return m_instance;
}

rdt::SceneID rdt::scene::SceneManager::RegisterScene(const char* sceneName, Scene* scene)
{
	return Get()->RegisterSceneImpl(sceneName, scene);
}

rdt::SceneID rdt::scene::SceneManager::GetSceneID(const char* sceneName)
{
	if (Get()->SceneExists(sceneName)) {
		return Get()->GetSceneIDImpl(sceneName);
	}

	return RDT_NULL_SCENE_ID;
}

void rdt::scene::SceneManager::SetScene(const char* sceneName)
{
}

rdt::SceneID rdt::scene::SceneManager::RegisterSceneImpl(const char* sceneName, Scene* scene)
{
	if (SceneExists(sceneName)) {
		RDT_WARN("SceneManager - Scene duplicate found! ({})", sceneName);
		RDT_CORE_WARN("SceneManager - Tried to registered duplicate scene: {}", sceneName);
		delete scene;
		return GetSceneIDImpl(sceneName);
	}

	if (sceneName == "") {
		RDT_WARN("SceneManager - Empty scene names not allowed!");
		RDT_CORE_WARN("SceneManager - Tried to registered duplicate scene: {}", sceneName);
		delete scene;
		return RDT_NULL_SCENE_ID;
	}


	SceneID nID = NextSceneID();
	scene->SetSceneID(nID);
	scene->SetSceneName(sceneName);
	sceneAliasToId[sceneName] = nID;
	m_scenes[nID] = scene;
	return nID;
}

rdt::SceneID rdt::scene::SceneManager::GetSceneIDImpl(const char* sceneName)
{
	// Precondition: Checked if exist already
	return sceneAliasToId.at(sceneName);
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
