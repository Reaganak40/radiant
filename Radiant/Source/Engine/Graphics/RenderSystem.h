#pragma once
#include "Core.h"
#include "ECS/ECS.h"

// Forward Declarations
namespace rdt {
	struct Renderable;
	struct Animator;
}

namespace rdt {

	class RADIANT_API RenderSystem : public systems::RenderUpdate {
	private:

	public:
		RenderSystem();
		~RenderSystem();

		void Update(float deltaTime = 0) override final;

	private:
		void ApplyAnimator(Renderable& renderable, Animator& animator);
	};
}