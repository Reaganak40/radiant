#include "pch.h"
#include "RenderSystem.h"
#include "Renderer.h"

rdt::RenderSystem::RenderSystem()
{
	Signature nSignature;
	
	// Components utilized by this system
	ComponentManager::UpdateSignature<Sprite>(nSignature);
	ComponentManager::UpdateSignature<Renderable>(nSignature);

	SetSignature(nSignature);
}

rdt::RenderSystem::~RenderSystem()
{
}

void rdt::RenderSystem::Update(float deltaTime)
{
	auto renderables = GetComponent<Renderable>();
	auto sprites = GetComponent<Sprite>();

	for (auto& entity : GetEntities()) {

		if (!renderables->HasEntity(entity)) {
			continue;
		}
		Renderable& renderable = renderables->GetData(entity);


		if (!sprites->HasEntity(entity)) {
			continue;
		}
		Sprite& sprite = sprites->GetData(entity);

		Renderer::Begin(renderable.layer);
		Renderer::SetPolygonColor(renderable.polygon_color);
		Renderer::AddPolygon(*sprite.polygon);
		Renderer::End();
	}
}
