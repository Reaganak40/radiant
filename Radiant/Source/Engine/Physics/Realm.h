/*******************************************************************
*	Module:  Physics (core)
*	File:    Realm.h
*
*	Author: Reagan Kelley
*
*   Realms act as a container which defines a physics environment.
*	That being the physical properties of the environment, and
*	the entities that reside within it.
*******************************************************************/
#pragma once

// Forward Declarations
namespace rdt {
	struct RigidBody2D;
}


// Required Definitions for Struct/Class Members
#include "ECS/EntityManager.h"

#include "Polygon/Polygon.h"
#include "Utils/UniqueID.h"
#include "Pobject.h"
#include "Messaging/MessageBus.h"

namespace rdt::core {

	class Realm {
	private:
		std::vector<Entity> m_entities;

		bool is_active;
		Vec2d m_gravity;
	public:
		Realm();
		~Realm();

		/*
			Adds an entity to the physics realm
		*/
		void PushEntity(Entity nEntity);

		/*
			Resets the entity buffer to size 0.
		*/
		void Flush();

		/*
			Turns the realm on or off. When not active, no updates
			are made.
		*/
		void SetActive(bool active);

		/*
			Move one timestep in the realm
		*/
		void OnUpdate(const float deltaTime);

	private:

		void OnEndFrame();
		
		const UniqueID GetUUID() { return m_ID; }

		const UniqueID CreatePhysicsObject(std::shared_ptr<Polygon> polygon, const MessageID messageID);
		std::shared_ptr<Polygon> DestroyPhysicsObject(const UniqueID UUID);

		Pobject* GetPhysicsObject(const UniqueID UUID);

		void SetGravity(double mps2);
		
		friend class Physics;
	};
}