#include "pch.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneManager.h"

#include <Radiant/Logger.h>

struct rdt::scene::Scene::Impl {

	SceneID m_ID = RDT_NULL_SCENE_ID;
	std::string m_name;

	std::vector<Layer*> m_layer_stack;
	bool is_bound = false;

	Impl()
	{
	}

	~Impl()
	{
	}

	void RemoveFromLayerStack(LayerID lID)
	{
		size_t index = 0;
		for (auto layer : m_layer_stack) {
			if (layer->GetID() == lID) {
				m_layer_stack.erase(m_layer_stack.begin() + index);
				return;
			}
			index++;
		}

		RDT_CORE_WARN("Scene - LayerID [{}] not found in layer stack (could not remove)", lID);
	}

	void AddToLayerStack(Layer* layer, size_t index)
	{
		if (layer == nullptr) {
			return;
		}
		m_layer_stack.insert(m_layer_stack.begin() + index, layer);
	}

	void BindLayers()
	{
		for (auto layer : m_layer_stack) {
			layer->Bind();
		}
	}

	void ReleaseLayers()
	{
		for (auto layer : m_layer_stack) {
			layer->Release();
		}
	}

	void DetachAll() {
		if (is_bound) {
			ReleaseLayers();
		}

		for (auto layer : m_layer_stack) {
			layer->Detach();
		}

		m_layer_stack.clear();
	}
};

// =============================================
rdt::scene::Scene::Scene()
	: m_impl(new Scene::Impl)
{
}

rdt::scene::Scene::~Scene()
{
	if (IsBound()) {
		Release();
	}

	delete m_impl;
}

rdt::SceneID rdt::scene::Scene::GetID()
{
	return m_impl->m_ID;
}

const char* rdt::scene::Scene::GetName()
{
	return m_impl->m_name.c_str();
}

bool rdt::scene::Scene::IsBound()
{
	return m_impl->is_bound;
}

void rdt::scene::Scene::AttachLayer(const char* layerName)
{
	Layer* layer = SceneManager::AttachLayerToScene(layerName, m_impl->m_ID);
	m_impl->AddToLayerStack(layer, m_impl->m_layer_stack.size());
}

void rdt::scene::Scene::AttachLayer(const char* layerName, size_t insertIndex)
{
	Layer* layer = SceneManager::AttachLayerToScene(layerName, m_impl->m_ID);
	m_impl->AddToLayerStack(layer, insertIndex);
}

rdt::SceneID rdt::scene::Scene::RegisterSceneImpl(const char* sceneName, Scene* nScene)
{
	return SceneManager::RegisterScene(sceneName, nScene);
}

void rdt::scene::Scene::SetSceneID(SceneID nID)
{
	m_impl->m_ID = nID;
}

void rdt::scene::Scene::SetSceneName(const char* nName)
{
	m_impl->m_name = nName;
}

void rdt::scene::Scene::RemoveFromLayerStack(LayerID layer)
{
	m_impl->RemoveFromLayerStack(layer);
}

void rdt::scene::Scene::DetachAll()
{
	m_impl->DetachAll();
}

void rdt::scene::Scene::Bind()
{
	if (IsBound()) {
		RDT_CORE_WARN("Scene - Tried to bind already bound scene '{}'", m_impl->m_name);
		return;
	}

	OnBind(); // child implementation
	m_impl->BindLayers();
	m_impl->is_bound = true;
}

void rdt::scene::Scene::Release()
{
	if (!IsBound()) {
		RDT_CORE_WARN("Scene - Tried to release unbound scene '{}'", m_impl->m_name);
		return;
	}

	m_impl->is_bound = false;
	m_impl->ReleaseLayers();
	OnRelease(); // child implementation
}

void rdt::scene::Scene::ProcessInput(const float deltaTime)
{
	for (auto layer : m_impl->m_layer_stack) {
		layer->ProcessInput(deltaTime);
	}
}

void rdt::scene::Scene::WorldUpdate(const float deltaTime)
{
	for (auto layer : m_impl->m_layer_stack) {
		layer->WorldUpdate(deltaTime);
	}
}

void rdt::scene::Scene::FinalUpdate()
{
	for (auto layer : m_impl->m_layer_stack) {
		layer->FinalUpdate();
	}
}

void rdt::scene::Scene::RenderUpdate()
{
	for (auto layer : m_impl->m_layer_stack) {
		layer->RenderUpdate();
	}
}

rdt::SceneID rdt::scene::GetSceneID(const char* sceneName)
{
	return SceneManager::GetSceneID(sceneName);
}

void rdt::scene::SetScene(const char* sceneName)
{
	SceneManager::SetScene(sceneName);
}

void rdt::scene::SetScene(SceneID sceneID)
{
}

void rdt::scene::CallUpdate(LoopPhase step, float deltaTime)
{
	SceneManager::CallUpdate(step, deltaTime);
}

void rdt::scene::TearDown()
{
	SceneManager::TearDown();
}
