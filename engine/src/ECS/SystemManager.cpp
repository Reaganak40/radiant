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

	struct SystemContainer {
		std::vector<std::shared_ptr<System>> layer_systems;
		std::vector<std::shared_ptr<System>> universal_systems;
	};

	SystemContainer processInput_systems;
	SystemContainer worldUpdate_systems;
	SystemContainer finalUpdate_systems;
	SystemContainer renderUpdate_systems;

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

void rdt::ecs::SystemManager::RegisterSystem(SystemID sID, System* nSystem)
{

}
