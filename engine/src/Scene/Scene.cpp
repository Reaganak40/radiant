#include "pch.h"
#include <Radiant/Scene/Scene.hpp>
#include <Core/Utils/Error.hpp>

#include "SceneImpl.hpp"
#include "SceneManager.hpp"

static rdt::Scene* m_current_scene = nullptr;

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
	Layer* layer = SceneManager::AttachLayerToScene(layerName, m_impl->m_ID);
	m_impl->AddToLayerStack(layer, m_impl->m_layer_stack.size());
}

void rdt::Scene::AttachLayer(const char* layerName, size_t insertIndex)
{
	Layer* layer = SceneManager::AttachLayerToScene(layerName, m_impl->m_ID);
	m_impl->AddToLayerStack(layer, insertIndex);
}

rdt::SceneID rdt::scene::Scene::RegisterSceneImpl(const char* sceneName, Scene* nScene)
{
	return SceneManager::RegisterScene(sceneName, nScene);
}

rdt::scene::SceneImpl& rdt::Scene::GetImpl()
{
	return *m_impl;
}


void rdt::Scene::Bind()
{
	if (IsBound()) {
		RDT_CORE_WARN("Scene - Tried to bind already bound scene '{}'", m_impl->m_name);
		return;
	}

	OnBind(); // child implementation
	m_impl->BindLayers();
	m_impl->is_bound = true;
}

void rdt::Scene::Release()
{
	if (!IsBound()) {
		RDT_CORE_WARN("Scene - Tried to release unbound scene '{}'", m_impl->m_name);
		return;
	}

	m_impl->is_bound = false;
	m_impl->ReleaseLayers();
	OnRelease(); // child implementation
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

void rdt::Scene::SetCurrentScene(const char* sceneName)
{
	scene::SceneManager::
}

rdt::Scene& rdt::Scene::GetCurrentScene()
{
	if (m_current_scene == nullptr) {
		RDT_THROW_NOT_ASSIGNED_EXCEPTION("Scene - current scene pointer was null");
	}

	return *m_current_scene;
}


