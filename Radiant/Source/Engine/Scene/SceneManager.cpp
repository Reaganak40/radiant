#include "pch.h"
#include "SceneManager.h"
#include "Messaging/MessageTypes.h"

namespace rdt {
	SceneManager* SceneManager::m_instance = nullptr;

	SceneManager::SceneManager()
		: m_current_scene(nullptr), m_currentSceneName("")
	{
		m_scenes[""] = new Scene;
		m_scenes.at("")->OnRegister();

		RegisterToMessageBus("SceneManager");
		m_broadcast = MessageBus::CreateBroadcast("SceneManager", new Broadcast);
	}

	SceneManager::~SceneManager()
	{
		for (auto& [sceneName, scenePtr] : m_scenes) {
			delete scenePtr;
		}
	}

	void SceneManager::OnMessage(Message msg)
	{
		switch (msg.type) {
		case MT_RequestScenePtr:
			SendDirectMessage(msg.from, MT_SendScenePtr, new ScenePtrData(m_current_scene));
			break;
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
			RDT_CORE_WARN("SceneManager - Scene duplicate found!");
			return;
		}

		if (sceneName == "") {
			RDT_CORE_WARN("SceneManager - Empty scene names not allowed!");
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
			MessageBus::AddToBroadcast(m_broadcast, MT_SceneChanged, new SceneChangedData(m_current_scene));
		}

		return m_current_scene;
	}
}