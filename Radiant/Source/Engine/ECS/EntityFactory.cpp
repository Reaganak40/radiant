#include "pch.h"
#include "EntityFactory.h"
#include "Polygon/Rect.h"
#include "Graphics/Model.h"

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

	void EntityDefinition::SetEntityAlias(const std::string& alias)
	{
		m_impl->entityAlias = alias;
	}

	void EntityDefinition::SetEntityOwner(Layer* layer)
	{
		m_impl->entityOwner = layer;
	}

	Entity EntityFactory::Create(EntityDefinition* entityDef)
	{
		entityDef->OnCreate();
		Entity res = entityDef->GetID();
		delete entityDef;
		return res;
	}

	// ============================================================================

	struct SpawnRect::Impl {
		Vec2d spawnPos;
		Vec2d size;

		Impl(double xPos, double yPos, double width, double height)
		{
			spawnPos.x = xPos;
			spawnPos.y = yPos;
			size.x = width;
			size.y = height;
		}

		~Impl()
		{
		}

		std::shared_ptr<Polygon> CreatePtr()
		{
			return std::make_shared<Rect>(spawnPos, size.x, size.y);
		}
	};

	SpawnRect::SpawnRect(double xPos, double yPos, double width, double height)
		: m_impl(new SpawnRect::Impl(xPos, yPos, width, height))
	{
		
	}
	SpawnRect::~SpawnRect()
	{
		delete m_impl;
	}
	const Vec2d& SpawnRect::GetSpawnPos()
	{
		return m_impl->spawnPos;
	}
	const Vec2d& SpawnRect::GetSize()
	{
		return m_impl->size;
	}
	Sprite SpawnRect::CreateSprite()
	{
		Sprite res;
		res.model = RDT_NULL_MODEL_ID;
		return res;
	}
	Transform SpawnRect::CreateTransform()
	{
		Transform transform;
		transform.position = m_impl->spawnPos;
		transform.rotation = 0;
		transform.scale = m_impl->size;

		return transform;
	}
}
