#include "pch.h"
#include <Radiant/Scene/Layer.hpp>
#include <Radiant/Scene/Scene.hpp>
#include "SceneManagerImpl.hpp"
#include "LayerImpl.hpp"

#include <Radiant/Logger.hpp>
#include <Radiant/ECS/Entity.hpp>

rdt::Layer::Layer()
	: m_impl(new scene::LayerImpl)
{
}

rdt::Layer::~Layer()
{
	if (IsBound()) {
		Release();
	}

	if (IsAttached()) {
		Detach();
	}

	delete m_impl;
}

rdt::LayerID rdt::Layer::GetID()
{
	return m_impl->m_ID;
}

const char* rdt::Layer::GetName()
{
	return m_impl->m_name.c_str();
}

bool rdt::Layer::IsAttached()
{
	return m_impl->m_owner != RDT_NULL_SCENE_ID;
}

bool rdt::Layer::IsBound()
{
	return m_impl->is_bound;
}

void rdt::Layer::ChangeScene(const char* nScene)
{
	scene::SceneManagerImpl::Get().RequestToChangeScene(nScene);
}

rdt::EntityID rdt::Layer::AddEntityImpl(EntityDefinition& entityDef)
{
	entityDef.OnCreate();
	
	return 0;
}

rdt::LayerID rdt::Layer::RegisterLayerImpl(const char* layerName, Layer* nLayer)
{
	return scene::SceneManagerImpl::Get().RegisterLayer(layerName, nLayer);
}

rdt::scene::LayerImpl& rdt::Layer::GetImpl()
{
	return *m_impl;
}

void rdt::Layer::Attach(SceneID sID)
{
	m_impl->m_owner = sID;
	OnAttach();
}

void rdt::Layer::Detach()
{
	OnDetach();
	m_impl->m_owner = RDT_NULL_SCENE_ID;
}

void rdt::Layer::Bind()
{
	if (IsBound()) {
		RDT_CORE_WARN("Layer - Tried to double-bind '{}'", m_impl->m_name);
		return;
	}

	OnBind();
	m_impl->is_bound = true;
}

void rdt::Layer::Release()
{
	if (!IsBound()) {
		// no warning because there are cases when some layers are not bound
		// and ReleaseAllLayers is called.
		return;
	}

	OnRelease();
	m_impl->is_bound = false;
}

void rdt::Layer::ProcessInput(const float deltaTime)
{
	OnProcessInput(deltaTime);
}

void rdt::Layer::WorldUpdate(const float deltaTime)
{
	OnUpdateWorld(deltaTime);
}

void rdt::Layer::FinalUpdate()
{
	OnFinalUpdate();
}

void rdt::Layer::RenderUpdate()
{
	OnRenderUpdate();
}
