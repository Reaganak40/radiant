/*******************************************************************
*	Module:  Graphics (API)
*	File:    AnimationSystem.h
*
*	Author: Reagan Kelley
*
*   The AnimationSystem is a core ECS system that updates renderer
*	animator components.
*******************************************************************/
#pragma once
#include "Core.h"
#include "ECS/ECS.h"

namespace rdt {

	/*
		When an entity registers to this system, this will
		control its animator component.
	*/
	class AnimationSystem : public systems::WorldUpdate {
	private:
	public:
		AnimationSystem();
		~AnimationSystem();

		void Update(float deltaTime = 0) override final;
	};
}