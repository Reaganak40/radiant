#include "pch.h"
#include "SceneManager.h"

namespace rdt {
	SceneManager* SceneManager::m_instance = nullptr;

	SceneManager::SceneManager()
		: m_current_scene(nullptr), m_currentSceneName("")
	{
		m_scenes[""] = new Scene;
		m_scenes.at("")->OnRegister();
	}

	SceneManager::~SceneManager()
	{
		for (auto& [sceneName, scenePtr] : m_scenes) {
			delete scenePtr;
		}
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

	void SceneManager::RegisterScene(const std::string& sceneName, Scene* scene)
	{
		if (m_instance->m_scenes.find(sceneName) != m_instance->m_scenes.end()) {
			printf("Warning: Scene duplicate found!\n");
			return;
		}

		if (sceneName == "") {
			printf("Warning: Empty scene names not allowed!\n");
			return;
		}

		m_instance->m_scenes[sceneName] = scene;
		scene->OnRegister();
	}

	void SceneManager::SetScene(const std::string& sceneName)
	{
		m_instance->m_currentSceneName = sceneName;
	}

	Scene* SceneManager::GetAnyScene(const std::string& sceneName)
	{
		if (m_instance->m_scenes.find(sceneName) == m_instance->m_scenes.end()) {
			return nullptr;
		}

		return m_instance->m_scenes.at(sceneName);
	}

	Scene* SceneManager::GetCurrentSceneImpl()
	{
		if (m_current_scene != m_scenes[m_currentSceneName]) {
			if (m_current_scene != nullptr) {
				m_current_scene->OnRelease();
			}
			m_current_scene = m_scenes[m_currentSceneName];
			m_current_scene->OnBind();
		}

		return m_current_scene;
	}
}