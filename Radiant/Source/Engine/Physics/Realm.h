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

		/*
			Sets the y-gravitational vector
		*/
		void SetGravity(double mps2);
	};
}