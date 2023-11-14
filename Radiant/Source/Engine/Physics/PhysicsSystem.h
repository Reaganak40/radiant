/*******************************************************************
*	Module:  Physics/ECS (API)
*	File:    PhysicsSystem.h
*
*	Author: Reagan Kelley
*
*   The file contains the physics system, which utlizes the internal
*	physics engine with the entity component system to simulate
*	real-world physics.
*******************************************************************/
#pragma once
#include "Core.h"
#include "Engine/ECS/SystemManager.h"

namespace rdt {

	class RADIANT_API PhysicsSystem : public systems::WorldUpdate {
	private:
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update(float deltaTime) override final;
	};

}