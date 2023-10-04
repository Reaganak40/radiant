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
			m_layers.push_back(core::DevLayer::GetInstance());
		}
	}

	Scene::~Scene()
	{
		FreeUniqueID(m_ID);

		for (int i = 0; i < m_layers.size() - 1; i++) {
			delete m_layers[i];
		}

		if (!ADD_DEV_LAYER) {
			delete m_layers.back();
		}
	}

	void Scene::RunProcessInputQueue(const float deltaTime)
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			if (!(*it)->IsAttached()) {
				continue;
			}

			(*it)->OnProcessInput(deltaTime);
		}
	}

	void Scene::RunFinalUpdateQueue()
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			if (!(*it)->IsAttached()) {
				continue;
			}

			(*it)->OnFinalUpdate();
		}
	}

	void Scene::RunRenderQueue()
	{
		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			if (!(*it)->IsAttached()) {
				continue;
			}

			(*it)->OnRender();
		}
	}

	void Scene::ChangeScene(const std::string& nScene)
	{
		SceneManager::SetScene(nScene);
	}

	void Scene::AddLayer(Layer* nLayer)
	{
		if (ADD_DEV_LAYER) {
			m_layers.insert(m_layers.begin() + (m_layers.size() - 1), nLayer);
		}
		else {
			m_layers.push_back(nLayer);
		}
	}
	void Scene::OnBind()
	{
		for (auto& layer : m_layers) {
			layer->OnAttach();
			layer->SetAttached(true);
		}
	}
	void Scene::OnRelease()
	{
		for (auto& layer : m_layers) {
			layer->OnDetach();
			layer->SetAttached(false);
		}
	}
}