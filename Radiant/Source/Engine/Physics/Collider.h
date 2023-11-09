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
#include "Core.h"

// Forward Declarations
namespace rdt {
	struct Transform;
	namespace core {
		class Realm;
	}
}

// Required Definitions for Struct/Class Members
#include "Utils/MathTypes.h"

#define RDT_NULL_COLLIDER_ID 0

namespace rdt {

	using ColliderID = unsigned int;

	enum ColliderShape {
		CCS_Rect
	};

	class Collider {
	private:
		std::vector<Vec2f> m_vertices;
		Vec2d m_size;
		Vec2d m_midpoint;

		bool m_is_rect;
		bool m_is_axis_aligned;
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

		/*
			Returns true if this collider is a Rect hitbox (capatible of AABB and other
			rect collision algorithms)
		*/
		bool IsRect();

		/*
			Returns true if the hitbox has all axis aligned edges
		*/
		bool IsAxisAligned();

		/*
			Returns the width and height dimensions of the collider. Option to scale it.
		*/
		Vec2d GetSize(Vec2d scale = Vec2d(1, 1));

		/*
			Returns the centered point of the collider shape, according to the given
			transform.
		*/
		Vec2d GetMidpoint(const Transform& transform);

	private:
		void WarningUndefinedShape();

		void AddRect(const Vec2f& origin, const Vec2f& size);
	};

	class RADIANT_API ColliderManager {
		struct Impl;
		static Impl* m_impl;

	public:

		/*
			Creates a new instance of the ColliderManager singleton
		*/
		static void Initialize();

		/*
			Frees the singleton instance
		*/
		static void Destroy();

		/*
			Gets the colliderID belonging to the the given alias,
			returns 0 if it is not found.
		*/
		static ColliderID GetColliderID(const std::string& alias);

		/*
			Returns true if the collider with the given ID exists
		*/
		static bool ColliderExists(ColliderID cpID);
		
		/*
			Gets the name for the colldier with the given ID
		*/
		static const char* GetColliderAlias(ColliderID cID);
		
		friend class core::Realm;
		friend class ResourceManager;
	private:

		/*
			Registers a collider under the given alias, returns the new collider's ID
		*/
		static ColliderID RegisterColider(const std::string& name);

		/*
			Gets a collider through the given ID
		*/
		static Collider& GetCollider(ColliderID cpID);
	};
}