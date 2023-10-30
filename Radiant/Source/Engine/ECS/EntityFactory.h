#pragma once
#include "Core.h"
#include "EntityManager.h"

namespace rdt {

	class RADIANT_API EntityDefinition {
	private:
		Entity* mID;
	public:
		EntityDefinition() : mID(new Entity(NO_ENTITY_ID)) {}
		~EntityDefinition() { delete mID; }

		virtual void OnCreate() = 0;

		/*
			Gets the entityID for this registered entity
		*/
		const Entity GetID() const;

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
}