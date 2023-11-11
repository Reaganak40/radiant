#include "pch.h"
#include "SceneManager.h"
#include "Messaging/MessageBus.h"
#include "Messaging/MessageTypes.h"

#include "Logging/Log.h"

namespace rdt {
	SceneManager* SceneManager::m_instance = nullptr;

	SceneManager::SceneManager()
		: m_current_scene(nullptr), m_currentSceneName("")
	{
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

	void SceneManager::RegisterScene(const std::string& sceneName, std::shared_ptr<Scene> scene)
	{
		if (m_instance->m_scenes.find(sceneName) != m_instance->m_scenes.end()) {
			RDT_CORE_WARN("SceneManager - Scene duplicate found!");
			return;
		}

		if (sceneName == "") {
			RDT_CORE_WARN("SceneManager - Empty scene names not allowed!");
			return;
		}

		m_instance->m_scenes[sceneName] = scene;
	}

	void SceneManager::SetScene(const std::string& sceneName)
	{
		m_instance->m_currentSceneName = sceneName;
	}

	std::shared_ptr<Scene> SceneManager::GetCurrentSceneImpl()
	{
		if (m_currentSceneName == "" && m_scenes.find("") == m_scenes.end()) {
			m_scenes[""] = std::make_shared<Scene>();
		}

		if (m_current_scene != m_scenes[m_currentSceneName]) {
			if (m_current_scene != nullptr) {
				m_current_scene->Release();
			}
			m_current_scene = m_scenes[m_currentSceneName];
			m_current_scene->Bind();
		}

		return m_current_scene;
	}
}