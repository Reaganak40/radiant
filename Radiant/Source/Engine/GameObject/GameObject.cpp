#include "pch.h"
#include "GameObject.h"
#include "Physics/Physics.h"
#include "Logging/Log.h"

namespace rdt {

	struct GameObject::Impl {
		UniqueID m_ID;
		UniqueID m_layerID;
		UniqueID m_realmID;

		UniqueID m_model_ID;
		GameState GState;

		std::shared_ptr<Polygon> m_cached_polygon;

		bool m_isBinded;

		Impl()
			: m_ID(GetUniqueID()), m_layerID(0), m_model_ID(0), m_realmID(0)
		{
			m_cached_polygon.reset();
			m_isBinded = false;
		}

		~Impl()
		{
		}
	};

	void GameObject::RemoveObjectFromWorld()
	{
		if (m_impl->m_realmID == 0) {
			RDT_CORE_WARN("GameObject - Could not remove object with RealmID: 0");
			return;
		} else if (m_impl->m_model_ID == 0) {
			RDT_CORE_WARN("GameObject - Could not remove object with ModelID: 0");
			return;
		}

		m_impl->m_cached_polygon = Physics::RemoveObject(m_impl->m_realmID, m_impl->m_model_ID);
		m_impl->m_model_ID = 0;
	}

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

	const bool GameObject::IsBinded()
	{
		return m_impl->m_isBinded;
	}

	void GameObject::OnBind()
	{
		m_impl->m_isBinded = true;
	}

	void GameObject::OnRelease()
	{
		m_impl->m_isBinded = false;
	}

	void GameObject::AddObjectToWorld(std::shared_ptr<Polygon> polygon)
	{
		if (m_impl->m_realmID == 0) {
			RDT_CORE_WARN("GameObject - Could not add object with RealmID: 0");
			return;
		}

		if (m_impl->m_model_ID != 0) {
			RDT_CORE_WARN("GameObject - Could not add object with in-use ModelID: {}", m_impl->m_model_ID);
			return;
		}

		if (polygon == nullptr) {
			if (m_impl->m_cached_polygon == nullptr) {
				RDT_CORE_WARN("GameObject - Tried to use cached polygon, but nothing is cached!");
				return;
			}
			polygon = m_impl->m_cached_polygon;
		}

		m_impl->m_model_ID = Physics::CreateObject(m_impl->m_realmID, GetMessageID(), polygon);
	}

	void GameObject::OnMessage(Message msg)
	{
		RDT_CORE_WARN("GameObject - Using Base Class OnMessage!");
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
