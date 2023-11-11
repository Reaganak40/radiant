#include "pch.h"
#include "Scene.h"
#include "Layer.h"
#include "Editor/DevTools.h"

#ifdef RDT_USE_DEV_TOOLS
#define ADD_DEV_LAYER 1
#else
#define ADD_DEV_LAYER 0
#endif

struct rdt::Scene::Impl {
	std::vector<std::shared_ptr<Layer>> m_layers;

	std::shared_ptr<Layer> devLayer = nullptr;
	Impl()
	{
		if (ADD_DEV_LAYER) {
			devLayer = core::DevLayer::GetInstance();
		}
	}

	~Impl()
	{
	}

	void AddLayer(std::shared_ptr<Layer> nLayer)
	{
		m_layers.push_back(nLayer);
	}

	std::vector<std::shared_ptr<Layer>>& GetLayers() {
		return m_layers;
	}
};

rdt::Scene::Scene()
	: m_impl(new Scene::Impl)
{
}

rdt::Scene::~Scene()
{
	delete m_impl;
}

void rdt::Scene::AddLayer(std::shared_ptr<Layer> nLayer)
{
	m_impl->AddLayer(nLayer);
}

void rdt::Scene::Bind()
{
	for (auto layer : m_impl->GetLayers()) {
		layer->OnAttach();
	}
}

void rdt::Scene::Release()
{
	for (auto layer : m_impl->GetLayers()) {
		layer->OnDetach();
	}
}

void rdt::Scene::ProcessInput(const float deltaTime)
{
	for (auto layer : m_impl->GetLayers()) {
		layer->PollMessages();
		layer->ProcessInput(deltaTime);
	}
}

void rdt::Scene::UpdateWorld(const float deltaTime)
{
	for (auto layer : m_impl->GetLayers()) {
		layer->PollMessages();
		layer->UpdateWorld(deltaTime);
	}
}

void rdt::Scene::FinalUpdate()
{
	for (auto layer : m_impl->GetLayers()) {
		layer->PollMessages();
		layer->FinalUpdate();
	}
}

void rdt::Scene::RenderUpdate()
{
	for (auto layer : m_impl->GetLayers()) {
		layer->PollMessages();
		layer->RenderUpdate();
	}
}
