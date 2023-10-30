#pragma once
#include "Core.h"
#include "ECS/ECS.h"

namespace rdt {

	class RADIANT_API PhysicsSystem : public systems::WorldUpdate {
	private:
	public:
		PhysicsSystem();
		~PhysicsSystem();

		void Update(float deltaTime) override final;

	};

}