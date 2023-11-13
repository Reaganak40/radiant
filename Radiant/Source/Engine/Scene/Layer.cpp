#include "pch.h"
#include "Layer.h"

#include "Messaging/Communicator.h"
#include "Messaging/MessageBus.h"
#include "Physics/Physics.h"
#include "ECS/EntityFactory.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

struct rdt::Layer::Impl {

	LayerID mID = RDT_NULL_LAYER_ID;

	bool m_attached;

	std::vector<RealmID> m_realms;
	std::vector<Entity> m_entities;

	Communicator m_communicator;
	std::set<ChannelID> m_subscribed_channels;
	ChannelID currentChannel = RDT_NULL_MESSAGE_ID;

	Scene* m_host_scene = nullptr;

	Impl()
		: m_attached(false)
	{
	}

	~Impl()
	{
		for (auto entity : m_entities) {
			EntityManager::RemoveEntity(entity);
		}
	}

	void SubscribeToChannel(ChannelID channel) {
		m_subscribed_channels.insert(channel);
	}

	void SetHostScene(Scene* scene) {
		m_host_scene = scene;
	}

	RealmID GetRealm(size_t realmIndex) {
		if (m_host_scene == nullptr) {
			return RDT_NULL_REALM_ID;
		}

		auto& realms = m_host_scene->GetRealms();

		if (realmIndex < realms.size()) {
			return realms[realmIndex];
		}

		return RDT_NULL_REALM_ID;
	}
};

// ==================================================================

rdt::Entity rdt::Layer::RegisterEntity(EntityDefinition* nEntity, const std::string& alias)
{
	nEntity->SetEntityOwner(this);
	if (!alias.empty()) {
		nEntity->SetEntityAlias(alias);
	}

	m_impl->m_entities.push_back(EntityFactory::Create(nEntity)); // frees nEntity

	return m_impl->m_entities.back();
}

rdt::Layer::Layer()
	: m_impl(new Layer::Impl)
{
}

rdt::Layer::~Layer()
{
	delete m_impl;
}

void rdt::Layer::BindToScene(Scene* scene)
{
	m_impl->SetHostScene(scene);
}

void rdt::Layer::SetLayerID(LayerID nID)
{
	m_impl->mID = nID;
}

rdt::LayerID rdt::Layer::GetID() 
{
	return m_impl->mID;
}

bool rdt::Layer::IsAttached()
{
	return m_impl->m_attached;
}

void rdt::Layer::ProcessInput(const float deltaTime)
{
	OnProcessInput(deltaTime);
}

void rdt::Layer::UpdateWorld(const float deltaTime)
{
	OnUpdateWorld(deltaTime);
}

void rdt::Layer::FinalUpdate()
{
	OnFinalUpdate();
}

void rdt::Layer::RenderUpdate()
{
	OnRenderUpdate();
}

void rdt::Layer::PollMessages()
{
	for (auto channel : m_impl->m_subscribed_channels) {
		m_impl->currentChannel = channel;
		m_impl->m_communicator.BeginPoll(channel);
		Message msg;

		while (m_impl->m_communicator.GetNextMessage(msg)) {
			OnMessage(msg);
		}
		m_impl->m_communicator.EndPoll();
	}

	m_impl->currentChannel = RDT_NULL_CHANNEL_ID;
}

void rdt::Layer::SubscribeToChannel(ChannelID channel)
{
	m_impl->SubscribeToChannel(channel);
}

rdt::ChannelID rdt::Layer::CurrentMessageChannel()
{
	return m_impl->currentChannel;
}

const std::vector<rdt::Entity>& rdt::Layer::GetEntities()
{
	return m_impl->m_entities;
}

rdt::RealmID rdt::Layer::GetRealm(size_t realmIndex)
{
	return m_impl->GetRealm(realmIndex);
}

void rdt::Layer::SetAttached(bool attach)
{
	m_impl->m_attached = attach;
}