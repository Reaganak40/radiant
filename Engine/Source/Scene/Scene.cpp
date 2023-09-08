#include "pch.h"
#include "Scene.h"

namespace Radiant {
	
	Scene::Scene()
		: m_ID(GetUniqueID())
	{
	}
	Scene::~Scene()
	{
		for (auto& object : m_game_objects) {
			delete object;
		}
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
