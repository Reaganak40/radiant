#pragma once
#include "Core.h"
#include "ComponentManager.h"

using EntityID = std::uint32_t;
#define NO_ENTITY_ID 0

namespace rdt {
	class RADIANT_API EntityManager {
	private:
		struct Impl;
		Impl* m_impl;
		
		EntityManager();
		~EntityManager();
		static EntityManager* m_instance;

	public:

		/*
			Create a new singleton instance of the EntityManager
		*/
		static void Initialize();

		/*
			Destroy the current singleton instance of the EntityManager
		*/
		static void Destroy();

		/*
			Create a new entity by registering the returned EntityID
		*/
		static EntityID RegisterEntity();

		/*
			Removes a registered entity that is referenced by the eID
		*/
		static void RemoveEntity(EntityID eID);

		/*
			Sets the component signature for this entity.
		*/
		static void SetSignature(EntityID eID, const Signature& signature);

		/*
			Gets the component signature for this entity.
		*/
		static Signature GetSignature(EntityID eID);
	};
}