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

	struct Scene::Impl {
		UniqueID m_ID;
		std::vector<Layer*> m_layers;
		bool m_use_default_camera;

		Impl()
			: m_ID(0), m_use_default_camera(true)
		{
			if (ADD_DEV_LAYER) {
				m_layers.push_back(core::DevLayer::GetInstance());
			}
		}

		~Impl()
		{
			FreeUniqueID(m_ID);

			for (int i = 0; i < m_layers.size() - 1; i++) {
				delete m_layers[i];
			}

			if (!ADD_DEV_LAYER) {
				delete m_layers.back();
			}
		}
	};

	Scene::Scene()
		: m_impl(new Scene::Impl)
	{
	}

	Scene::~Scene()
	{
		delete m_impl;
	}

	const UniqueID Scene::GetID()
	{
		return m_impl->m_ID;
	}

	void Scene::RunProcessInputQueue(const float deltaTime)
	{
		for (auto it = m_impl->m_layers.rbegin(); it != m_impl->m_layers.rend(); ++it)
		{
			if (!(*it)->IsAttached()) {
				continue;
			}

			(*it)->OnProcessInput(deltaTime);
		}
	}

	void Scene::RunFinalUpdateQueue()
	{
		for (auto it = m_impl->m_layers.rbegin(); it != m_impl->m_layers.rend(); ++it)
		{
			if (!(*it)->IsAttached()) {
				continue;
			}

			(*it)->OnFinalUpdate();
		}
	}

	void Scene::RunRenderQueue()
	{
		if (m_impl->m_use_default_camera) {
			Renderer::UseCamera(); // delcares using the default camera
		}

		for (auto it = m_impl->m_layers.rbegin(); it != m_impl->m_layers.rend(); ++it)
		{
			if (!(*it)->IsAttached()) {
				continue;
			}

			(*it)->OnRender();
		}
	}

	void Scene::DontUseDefaultCamera()
	{
		m_impl->m_use_default_camera = false;
	}

	void Scene::AddLayer(Layer* nLayer)
	{
		if (ADD_DEV_LAYER) {
			m_impl->m_layers.insert(m_impl->m_layers.begin() + (m_impl->m_layers.size() - 1), nLayer);
		}
		else {
			m_impl->m_layers.push_back(nLayer);
		}
	}
	void Scene::OnBind()
	{
		for (auto& layer : m_impl->m_layers) {
			layer->OnAttach();
			layer->SetAttached(true);
		}
	}
	void Scene::OnRelease()
	{
		for (auto& layer : m_impl->m_layers) {
			layer->OnDetach();
			layer->SetAttached(false);
		}
	}
	
	Layer** Scene::GetLayers(size_t* numLayers)
	{
		if (ADD_DEV_LAYER) {
			*numLayers = m_impl->m_layers.size() - 1;
		}
		else {
			*numLayers = m_impl->m_layers.size();
		}

		return m_impl->m_layers.data();
	}

}