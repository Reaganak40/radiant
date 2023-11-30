#include "pch.h"
#include <Radiant/ECS/SystemManager.hpp>

/***************************************************************
* Forward Declarations
***************************************************************/
namespace rdt {
	
	using Signature = std::bitset<RDT_MAX_COMPONENTS>;

	namespace ecs {
		struct SystemManagerImpl;
	}
}

struct rdt::ecs::SystemManagerImpl {

	SystemManagerImpl()
	{
	}

	~SystemManagerImpl()
	{
	}
};

// =======================================================

static rdt::ecs::SystemManagerImpl* m_impl = nullptr;

void rdt::ecs::SystemManager::Initialize()
{
	Destroy();
	m_impl = new SystemManagerImpl;
}

void rdt::ecs::SystemManager::Destroy()
{
	if (m_impl != nullptr)
	{
		delete m_impl;
		m_impl = nullptr;
	}
}
