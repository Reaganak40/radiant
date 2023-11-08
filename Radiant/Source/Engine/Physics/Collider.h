/*******************************************************************
*	Module:  Physics (core)
*	File:    Collider.h
*
*	Author: Reagan Kelley
*
*   Colliders are used to define hitboxes and models and physics
*	collision detection and resolution. This file contains the
*	Collider class which handles the internal logic of a
*	Collider and the ColliderManager.
*******************************************************************/
#pragma once

// Forward Declarations
namespace rdt {
	struct Transform;
	namespace core {
		class Realm;
	}
}

// Required Definitions for Struct/Class Members
#include "Utils/MathTypes.h"

#define RDT_NULL_COLLIDER_PROFILE_ID

namespace rdt {

	using ColliderID = unsigned int;

	enum ColliderShape {
		CCS_Rect
	};

	class Collider {
	private:
		std::vector<Vec2f> m_vertices;

	public:

		Collider();
		~Collider();


		/*
			Adds a hitbox shape at the given origin (normalized), and size (normalized). Origin
			is the bottom left-most corner of the shape.
		*/
		template<ColliderShape shape>
		void AddShape(const Vec2f& origin, const Vec2f& size) { WarningUndefinedShape(); }

		template<>
		void AddShape<CCS_Rect>(const Vec2f& origin, const Vec2f& size) { AddRect(origin, size); }

		/*
			Takes a transform, and an out reference, and returns the collider
			vertices according to that transform.
		*/
		void ApplyTransform(const Transform& transform, std::vector<Vec2d>& vertices);

	private:
		void WarningUndefinedShape();

		void AddRect(const Vec2f& origin, const Vec2f& size);
	};

	class ColliderManager {
		struct Impl;
		static Impl* m_impl;

	public:

		static void Initialize();
		
		static void Destroy();


		friend class core::Realm;
	private:

		static bool ColliderExists(ColliderID cpID);

		static ColliderID AddCollider(const Collider& profile);

		static Collider& GetCollider(ColliderID cpID);
	};
}