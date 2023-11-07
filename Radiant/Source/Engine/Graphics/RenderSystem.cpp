#include "pch.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "Animation.h"

rdt::RenderSystem::RenderSystem()
{
	Signature nSignature;
	
	// Components utilized by this system
	ComponentManager::UpdateSignature<Transform>(nSignature);
	ComponentManager::UpdateSignature<Sprite>(nSignature);
	ComponentManager::UpdateSignature<Renderable>(nSignature);
	ComponentManager::UpdateSignature<Animator>(nSignature);

	SetSignature(nSignature);
}

rdt::RenderSystem::~RenderSystem()
{
}

void rdt::RenderSystem::Update(float deltaTime)
{
	auto renderables = GetComponent<Renderable>();
	auto sprites = GetComponent<Sprite>();
	auto animators = GetComponent<Animator>();
	auto transforms = GetComponent<Transform>();

	for (auto& entity : GetEntities()) {

		if (!renderables->HasEntity(entity) ||
			!sprites->HasEntity(entity)     ||
			!transforms->HasEntity(entity)) 
		{
			continue;
		}
		Renderable& renderable = renderables->GetData(entity);
		Sprite& sprite = sprites->GetData(entity);
		Transform& transform = transforms->GetData(entity);

		if (animators->HasEntity(entity)) {
			ApplyAnimator(renderable, animators->GetData(entity));
		}

		Renderer::Begin(renderable.layer);
		Renderer::SetModel(sprite.model);
		Renderer::SetTransform(transform);
		Renderer::SetTexture(renderable.texture);

		if (renderable.)
		Renderer::FlipTextureHorizontal()

		Renderer::SetPolygonColor(renderable.polygon_color);
		Renderer::AddPolygon(*sprite.polygon);
		Renderer::End();
	}
}

void rdt::RenderSystem::ApplyAnimator(Renderable& renderable, Animator& animator)
{
	if (animator.animationID == RDT_NULL_ANIMATION_ID) {
		return;
	}

	renderable.texture = AnimationManager::GetAnimationTexture(animator.animationID);
	renderable.atlasProfile = AnimationManager::GetFrame(animator.animationID, animator.currentFrame);
}
