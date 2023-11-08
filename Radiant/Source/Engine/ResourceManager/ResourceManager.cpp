#include "pch.h"
#include "ResourceManager.h"

#include "Graphics/Model.h"

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

	auto& rectModel = ModelManager::GetModel(ModelManager::RegisterModel("common::rect"));
	rectModel.DefineCommon<Common::RECT>();

	rectModel = ModelManager::GetModel(1);

}
