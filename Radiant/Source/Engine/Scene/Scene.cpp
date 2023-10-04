#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Editor/DevTools.h"
#include "Graphics/Renderer.h"

#ifdef RDT_DEBUG
#define ADD_DEV_LAYER 1
#else
#define ADD_DEV_LAYER 0
#endif

namespace rdt {
	Scene::Scene()
		: m_ID(GetUniqueID()), m_use_default_camera(true)
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
		if (m_use_default_camera) {
			Renderer::UseCamera(); // delcares using the default camera
		}

		for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
		{
			if (!(*it)->IsAttached()) {
				continue;
			}

			(*it)->OnRender();
		}
	}

	void Scene::DontUseDefaultCamera()
	{
		m_use_default_camera = false;
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