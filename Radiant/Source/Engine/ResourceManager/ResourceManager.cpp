#include "pch.h"
#include "ResourceManager.h"

#include "Graphics/Model.h"
#include "Physics/Collider.h"

struct rdt::ResourceManager::Impl {
	
	Impl()
	{
	}

	~Impl()
	{
	}
};

// =======================================================================

rdt::ResourceManager::Impl* rdt::ResourceManager::m_impl = nullptr;

void rdt::ResourceManager::Initialize()
{
	Destroy();
	m_impl = new ResourceManager::Impl;
}

void rdt::ResourceManager::Destroy()
{
	if (m_impl != nullptr) {
		delete m_impl;
		m_impl = nullptr;
	}
}

void rdt::ResourceManager::LoadDefaultResources()
{
	auto& rectModel = ModelManager::GetModel(ModelManager::RegisterModel("common.model.rect"));
	rectModel.AddShape<ModelShape::MS_RECT>();

	auto& rectCollider = ColliderManager::GetCollider(ColliderManager::RegisterColider("common.collider.rect"));
	rectCollider.AddShape<ColliderShape::CCS_Rect>(Vec2f::Zero(), Vec2f(1.0f, 1.0f));
}
