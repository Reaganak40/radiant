#include "pch.h"
#include "EntityFactory.h"

namespace rdt {
	void EntityDefinition::Register()
	{
		*mID = EntityManager::RegisterEntity();
	}

	const Entity EntityDefinition::GetID() const
	{
		return *mID;
	}

	Entity EntityFactory::Create(EntityDefinition* entityDef)
	{
		entityDef->OnCreate();
		Entity res = entityDef->GetID();
		delete entityDef;
		return res;
	}
}
