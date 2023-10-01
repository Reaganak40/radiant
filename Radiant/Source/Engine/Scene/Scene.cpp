#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Editor/DevTools.h"

#ifdef RDT_DEBUG
#define ADD_DEV_LAYER 1
#else
#define ADD_DEV_LAYER 0
#endif

namespace rdt {

	Scene::Scene()
		: m_ID(GetUniqueID())
	{
		if (ADD_DEV_LAYER) {
			m_layers.push_back(new core::DevLayer);
			m_layers.back()->OnAttach();
			RDT_CORE_WARN("Developer tools are enabled");
		}
	}

	Scene::~Scene()
	{
		FreeUniqueID(m_ID);
	}

	void Scene::RunProcessInputQueue(const float deltaTime)
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			(*it)->OnProcessInput(deltaTime);
		}
	}

	void Scene::RunFinalUpdateQueue()
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			(*it)->OnFinalUpdate();
		}
	}

	void Scene::RunRenderQueue()
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			(*it)->OnRender();
		}
	}

	void Scene::ChangeScene(const std::string& nScene)
	{
		SceneManager::SetScene(nScene);
	}

	void Scene::AttachLayer(Layer* nLayer)
	{
		if (ADD_DEV_LAYER) {
			m_layers.insert(m_layers.begin() + (m_layers.size() - 1), nLayer);
		}
		else {
			m_layers.push_back(nLayer);
		}

		nLayer->OnAttach();
	}
}