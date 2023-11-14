#include "pch.h"
#include "Scene.h"
#include "Layer.h"
#include "Physics/Physics.h"
#include "SceneManager.h"

#ifdef RDT_USE_DEV_TOOLS
#define ADD_DEV_LAYER 1
#else
#define ADD_DEV_LAYER 0
#endif

struct rdt::Scene::Impl {
	SceneID mID = RDT_NULL_SCENE_ID;
	std::string name = "";

	std::vector<std::shared_ptr<Layer>> m_layers;
	std::shared_ptr<Layer> devLayer = nullptr;

	std::vector<RealmID> m_realms;

	Impl()
	{
	}

	~Impl()
	{
	}

	void AddLayer(std::shared_ptr<Layer> nLayer)
	{
		m_layers.push_back(nLayer);
		nLayer->OnCreate();
	}

	const std::vector<std::shared_ptr<Layer>>& GetLayers() const {
		return m_layers;
	}

	RealmID AddRealm() {
		m_realms.push_back(Physics::CreateRealm());
		return m_realms.back();
	}

	const std::vector<RealmID>& GetRealms() const {
		return m_realms;
	}

	void Bind()
	{
		for (auto realm : m_realms) {
			Physics::ActivateRealm(realm);
		}

		for (auto& layer : m_layers) {
			layer->OnAttach();
		}
	}

	void Release()
	{
		for (auto&& layer : m_layers) {
			layer->OnDetach();
		}

		for (auto realm : m_realms) {
			Physics::DeactivateRealm(realm);
		}
	}
};

// ============================================================

rdt::Scene::Scene()
	: m_impl(new Scene::Impl)
{
}

rdt::Scene::~Scene()
{
	delete m_impl;
}

rdt::RealmID rdt::Scene::AddRealm()
{
	return m_impl->AddRealm();
}

void rdt::Scene::SetSceneID(SceneID nID)
{
	m_impl->mID = nID;
}

void rdt::Scene::SetSceneName(const std::string& name)
{
	m_impl->name = name;
}

void rdt::Scene::AddLayer(std::shared_ptr<Layer> layer)
{
	layer->BindToScene(this);
	m_impl->AddLayer(layer);
}

rdt::SceneID rdt::Scene::GetID()
{
	return m_impl->mID;
}

const char* rdt::Scene::GetName()
{
	return m_impl->name.c_str();
}

void rdt::Scene::Bind()
{
	m_impl->Bind();
}

void rdt::Scene::Release()
{
	m_impl->Release();
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

const std::vector<rdt::RealmID>& rdt::Scene::GetRealms() const
{
	return m_impl->GetRealms();
}

const std::vector<std::shared_ptr<rdt::Layer>>& rdt::Scene::GetLayers() const
{
	return m_impl->GetLayers();
}
