#include "pch.h"
#include <Radiant/ECS/Entity.hpp>

namespace rdt {

	struct EntityDefinition::Impl {
		Entity mID = NO_ENTITY_ID;
		std::string entityAlias = "";
		Layer* entityOwner = nullptr;

		Impl() {}
		~Impl() {}

		void OnRegister() {
			mID = EntityManager::RegisterEntity(entityAlias, entityOwner);
		}
	};

	EntityDefinition::EntityDefinition()
		: m_impl(new EntityDefinition::Impl)
	{
	}

	EntityDefinition::~EntityDefinition()
	{
		delete m_impl;
	}

	void EntityDefinition::Register()
	{
		m_impl->OnRegister();
	}

	const Entity EntityDefinition::GetID() const
	{
		return m_impl->mID;
	}

	void EntityDefinition::SetEntityOwner(Layer* layer)
	{
		m_impl->entityOwner = layer;
	}
}
