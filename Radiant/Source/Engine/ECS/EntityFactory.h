#pragma once
#include "Core.h"
#include "EntityManager.h"
#include "CommonComponents.h"

namespace rdt {

	/*
		Wrapper class than includes a virtual OnCreate function. This
		is a to-be-implemented factory method that is used the EntityFactory
		for the creation of common entities.
	*/
	class RADIANT_API EntityDefinition {
	private:
		struct Impl;
		Impl* m_impl;

	public:
		EntityDefinition();
		~EntityDefinition();

		virtual void OnCreate() = 0;

		/*
			Gets the entityID for this registered entity
		*/
		const Entity GetID() const;

		/*
			Sets the alias of the entity that is about to be registered.
			Make sure this is called before Register()
		*/
		void SetEntityAlias(const std::string& alias);

		/*
			Sets the owner of the entity that is about to be registered.
			Make sure this is called before Register()
		*/
		void SetEntityOwner(Layer* layer);

	protected:
		/*
			Utility function that registers a new entity with the
			entity manager and stored the entityID in mID
		*/
		void Register();
	};

	/*
		Used to create common entities
	*/
	class RADIANT_API EntityFactory {
	public:

		/*
			Creates an entity using the EntityDefinition class.
			This function will delete this entity definition from
			the heap.
		*/
		static Entity Create(EntityDefinition* entityDef);
	};

	// =========================================================================

	/*
		Used in combination with EntityDefinition to define the size and position
		of new entities that utilize a rect sprite.
	*/
	class RADIANT_API SpawnRect {
	private:
		struct Impl;
		Impl* m_impl;

	public:
		SpawnRect(double xPos, double yPos, double width, double height);
		~SpawnRect();

	protected:
		/*
			Gets the spawn coordinates for this entity
		*/
		const Vec2d& GetSpawnPos();

		/*
			Gets the spawn size for this entity
		*/
		const Vec2d& GetSize();

		/*
			Creates a sprite object with the preconfigured data
			represented by this class. (its spawn coordinates and size)
		*/
		Sprite CreateSprite();

		/*
			Creates a transform object with the preconfigured data
			represented by this class. (its spawn coordinates and size)
		*/
		Transform CreateTransform();
	};
}