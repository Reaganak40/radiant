#include "pch.h"
#include "AnimationSystem.h"
#include "Animation.h"

namespace rdt {
	AnimationSystem::AnimationSystem()
	{
		Signature nSignature;

		// Components utilized by this system
		ComponentManager::UpdateSignature<Animator>(nSignature);

		SetSignature(nSignature);
	}
	AnimationSystem::~AnimationSystem()
	{
	}
	void AnimationSystem::Update(float deltaTime)
	{
		auto animators = GetComponent<Animator>();

		for (auto& entity : GetEntities()) {
			if (!animators->HasEntity(entity)) {
				continue;
			}

			Animator& animator = animators->GetData(entity);
			
			if (animator.animationID == RDT_NULL_ANIMATION_ID) {
				continue;
			}

			if (animator.timer.IsRunning()) {
				if (animator.timer.Update(deltaTime)) {
					animator.currentFrame++;

					if (animator.currentFrame >= AnimationManager::GetFrameCount(animator.animationID)) {
						animator.currentFrame = 0;
					}

					animator.timer.Start();
				}
			}
		}
	}
}
