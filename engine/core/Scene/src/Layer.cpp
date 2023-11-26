#include "pch.h"
#include "Layer.h"
#include "Scene.h"
#include "SceneManager.h"

#include <Radiant/Logger.h>

struct rdt::scene::Layer::Impl {
	
	LayerID m_ID = RDT_NULL_LAYER_ID;
	std::string m_name;

	SceneID m_owner = RDT_NULL_SCENE_ID;
	bool is_bound = false;

	Impl()
	{
	}

	~Impl()
	{
	}
};

// =====================================================

rdt::scene::Layer::Layer()
	: m_impl(new Layer::Impl)
{
}

rdt::scene::Layer::~Layer()
{
	if (IsBound()) {
		Release();
	}

	if (IsAttached()) {
		Detach();
	}

	delete m_impl;
}

rdt::LayerID rdt::scene::Layer::GetID()
{
	return m_impl->m_ID;
}

const char* rdt::scene::Layer::GetName()
{
	return m_impl->m_name.c_str();
}

bool rdt::scene::Layer::IsAttached()
{
	return m_impl->m_owner != RDT_NULL_SCENE_ID;
}

bool rdt::scene::Layer::IsBound()
{
	return m_impl->is_bound;
}

rdt::LayerID rdt::scene::Layer::RegisterLayerImpl(const char* layerName, Layer* nLayer)
{
	return SceneManager::RegisterLayer(layerName, nLayer);
}

void rdt::scene::Layer::SetLayerID(LayerID nID)
{
	m_impl->m_ID = nID;
}

void rdt::scene::Layer::SetName(const char* name)
{
	m_impl->m_name = name;
}

rdt::SceneID rdt::scene::Layer::GetAttachedSceneID()
{
	return m_impl->m_owner;
}

void rdt::scene::Layer::Attach(SceneID sID)
{
	m_impl->m_owner = sID;
	OnAttach();
}

void rdt::scene::Layer::Detach()
{
	OnDetach();
	m_impl->m_owner = RDT_NULL_SCENE_ID;
}

void rdt::scene::Layer::Bind()
{
	if (IsBound()) {
		RDT_CORE_WARN("Layer - Tried to double-bind '{}'", m_impl->m_name);
		return;
	}

	OnBind();
	m_impl->is_bound = true;
}

void rdt::scene::Layer::Release()
{
	if (!IsBound()) {
		RDT_CORE_WARN("Layer - Tried to release unbound layer '{}'", m_impl->m_name);
		return;
	}

	OnRelease();
	m_impl->is_bound = false;
}

void rdt::scene::Layer::ProcessInput(const float deltaTime)
{
	OnProcessInput(deltaTime);
}

void rdt::scene::Layer::WorldUpdate(const float deltaTime)
{
	OnUpdateWorld(deltaTime);
}

void rdt::scene::Layer::FinalUpdate()
{
	OnFinalUpdate();
}

void rdt::scene::Layer::RenderUpdate()
{
	OnRenderUpdate();
}
