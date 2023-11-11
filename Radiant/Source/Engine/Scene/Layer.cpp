#include "pch.h"
#include "Layer.h"

#include "Messaging/Communicator.h"
#include "Messaging/MessageBus.h"
#include "Physics/Physics.h"
#include "ECS/EntityFactory.h"

struct rdt::Layer::Impl {
	bool m_attached;

	std::vector<RealmID> m_realms;
	std::vector<Entity> m_entities;

	Communicator m_communicator;
	std::set<ChannelID> m_subscribed_channels;
	ChannelID currentChannel = RDT_NULL_MESSAGE_ID;

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
};

// ==================================================================

std::vector<rdt::RealmID>& rdt::Layer::GetRealms()
{
	return m_impl->m_realms;
}


rdt::RealmID rdt::Layer::CreateNewRealm()
{
	m_impl->m_realms.push_back(Physics::CreateRealm());
	return m_impl->m_realms.back();
}


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

void rdt::Layer::SetAttached(bool attach)
{
	m_impl->m_attached = attach;
}