#include "pch.h"
#include "EntityFactory.h"
#include "Polygon/Rect.h"

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
		res.polygon = m_impl->CreatePtr();
		return res;
	}
}
