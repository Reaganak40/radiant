#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"

namespace rdt {
	Scene::Scene()
		: m_ID(GetUniqueID())
	{
	}
	Scene::~Scene()
	{
		for (auto& object : m_game_objects) {
			delete object;
		}

		for (auto& gui: m_GUIs) {
			delete gui;
		}
	}

	void Scene::ChangeScene(const std::string& nScene)
	{
		SceneManager::SetScene(nScene);
	}

	Scene* Scene::GetOtherScene(const std::string& nScene) {
		return SceneManager::GetAnyScene(nScene);
	}

	void Scene::BeginExit() {
		SceneManager::UnselectScene();
	}

	void Scene::RunProcessInputQueue(const float deltaTime)
	{
		for (auto& object : m_game_objects) {
			object->OnProcessInput(deltaTime);
		}

		for (auto& gui : m_GUIs) {
			gui->OnUpdate(deltaTime);
		}
	}
	
	void Scene::RunFinalUpdateQueue()
	{
		for (auto& object : m_game_objects) {
			object->OnFinalUpdate();
		}
	}

	void Scene::RunRenderQueue()
	{
		for (auto& object : m_game_objects) {
			object->OnRender();
		}
	}

}
