#include "pch.h"
#include "RenderSystem.h"
#include "Renderer.h"

rdt::RenderSystem::RenderSystem()
{
	Signature nSignature;
	ComponentManager::UpdateSignature<Renderable>(nSignature);
	SetSignature(nSignature);
}

rdt::RenderSystem::~RenderSystem()
{
}

void rdt::RenderSystem::Update(float deltaTime)
{
	auto renderableData = GetComponent<Renderable>();

	for (auto& entity : GetEntities()) {

		Renderable& renderable = renderableData->GetData(entity);

		if (!renderable.IsUsingPolygon()) {
			continue;
		}

		Renderer::Begin(renderable.layer);
		Renderer::SetPolygonColor(renderable.polygon_color);
		Renderer::AddPolygon(*renderable.polygon);
		Renderer::End();
	}
}
