#include "pch.h"
#include <Radiant/Scene/Scene.hpp>
#include <Radiant/Scene/Layer.hpp>
#include <Core/Utils/Error.hpp>

#include "SceneImpl.hpp"
#include "SceneManagerImpl.hpp"


rdt::Scene::Scene()
	: m_impl(new scene::SceneImpl)
{
}

rdt::Scene::~Scene()
{
	if (IsBound()) {
		Release();
	}

	delete m_impl;
}

rdt::SceneID rdt::Scene::GetID()
{
	return m_impl->m_ID;
}

const char* rdt::Scene::GetName()
{
	return m_impl->m_name.c_str();
}

bool rdt::Scene::IsBound()
{
	return m_impl->is_bound;
}

void rdt::Scene::AttachLayer(const char* layerName)
{
	// handles the attaching and binding of the layer
	Layer* layer = scene::SceneManagerImpl::Get().AttachLayerToScene(layerName, m_impl->m_ID);

	if (layer != nullptr) {
		m_impl->AddToLayerStack(layer, m_impl->m_layer_stack.size());
	}
}

void rdt::Scene::AttachLayer(const char* layerName, size_t insertIndex)
{
	Layer* layer = scene::SceneManagerImpl::Get().AttachLayerToScene(layerName, m_impl->m_ID);
	m_impl->AddToLayerStack(layer, insertIndex);
}

void rdt::Scene::BindLayer(const char* layerName)
{
	m_impl->BindLayer(layerName);
}

void rdt::Scene::BindLayer(size_t layerIndex)
{
	m_impl->BindLayer(layerIndex);
}

void rdt::Scene::BindAllLayers()
{
	m_impl->BindLayers();
}

void rdt::Scene::ReleaseLayer(const char* layerName)
{
	m_impl->ReleaseLayer(layerName);
}

rdt::SceneID rdt::Scene::RegisterSceneImpl(const char* sceneName, Scene* nScene)
{
	return scene::SceneManagerImpl::Get().RegisterScene(sceneName, nScene);
}

rdt::scene::SceneImpl& rdt::Scene::GetImpl()
{
	return *m_impl;
}

void rdt::Scene::ReleaseLayer(size_t layerIndex)
{
	m_impl->ReleaseLayer(layerIndex);
}

void rdt::Scene::ChangeScene(const char* nScene)
{
	scene::SceneManagerImpl::Get().RequestToChangeScene(nScene);
}

void rdt::Scene::Bind()
{
	if (IsBound()) {
		RDT_CORE_WARN("Scene - Tried to bind already bound scene '{}'", m_impl->m_name);
		return;
	}

	m_impl->is_bound = true;
	OnBind(); // child implementation
}

void rdt::Scene::Release()
{
	if (!IsBound()) {
		RDT_CORE_WARN("Scene - Tried to release unbound scene '{}'", m_impl->m_name);
		return;
	}

	m_impl->is_bound = false;
	OnRelease(); // child implementation
	m_impl->ReleaseLayers();
}

void rdt::Scene::ProcessInput(const float deltaTime)
{
	for (auto layer : m_impl->m_layer_stack) {
		layer->ProcessInput(deltaTime);
	}
}

void rdt::Scene::WorldUpdate(const float deltaTime)
{
	for (auto layer : m_impl->m_layer_stack) {
		layer->WorldUpdate(deltaTime);
	}
}

void rdt::Scene::FinalUpdate()
{
	for (auto layer : m_impl->m_layer_stack) {
		layer->FinalUpdate();
	}
}

void rdt::Scene::RenderUpdate()
{
	for (auto layer : m_impl->m_layer_stack) {
		layer->RenderUpdate();
	}
}
