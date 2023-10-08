#include "pch.h"
#include "GameObject.h"
#include "Physics/Physics.h"

namespace rdt {

	struct GameObject::Impl {
		UniqueID m_ID;
		UniqueID m_layerID;
		UniqueID m_realmID;

		UniqueID m_model_ID;
		GameState GState;

		Impl()
			: m_ID(GetUniqueID()), m_layerID(0), m_model_ID(0), m_realmID(0)
		{
		}

		~Impl()
		{
		}
	};

	GameObject::GameObject()
		: m_impl(new GameObject::Impl)
	{
	}

	GameObject::~GameObject()
	{
		delete m_impl;
	}

	const UniqueID GameObject::GetID()
	{
		return m_impl->m_ID;
	}

	const UniqueID GameObject::GetLayerID()
	{
		return m_impl->m_layerID;
	}

	const UniqueID GameObject::GetRealmID()
	{
		return m_impl->m_realmID;
	}

	const UniqueID GameObject::GetModelID()
	{
		return m_impl->m_model_ID;
	}

	void GameObject::AddObjectToWorld(std::shared_ptr<Polygon> polygon)
	{
		m_impl->m_model_ID = Physics::CreateObject(m_impl->m_realmID, GetMessageID(), polygon);
	}

	void GameObject::OnMessage(Message msg)
	{
		printf("Warning: Using Base Class OnMessage!\n");
	}
	void GameObject::RegisterToLayer(const UniqueID nLayerID)
	{
		m_impl->m_layerID = nLayerID;
	}
	void GameObject::RegisterToRealm(const UniqueID nRealmID)
	{
		m_impl->m_realmID = nRealmID;
	}
}
