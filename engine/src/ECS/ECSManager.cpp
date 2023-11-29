#include "pch.h"
#include <Radiant/ECS/ECSManager.hpp>

#include "EntityManager.h"

void rdt::ecs::ECSManager::Initialize()
{
	EntityManager::Get();
}

void rdt::ecs::ECSManager::Destroy()
{
	EntityManager::Destroy();
}
