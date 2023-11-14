#pragma once
#include "Core.h"
#include "Utils/UniqueID.h"
#include "Realm.h"
#include "Messaging/MessageBus.h"

// Forward Declarations
namespace rdt {
	using Entity = unsigned int;
}

#define RDT_NULL_REALM_ID 0

namespace rdt {
	using RealmID = unsigned int;

	// =======================================================================================
	enum PhysicalProperties {
		PhysicalProperty_None = 0,	   // No special physical properties
		PhysicalProperty_NoCollision = (1 << 0), // Do not detect or resolve any collisions
		PhysicalProperty_Bouncy = (1 << 1), // When colliding with an object, bounce back.
		PhysicalProperty_DontResolve = (1 << 2), // Detect but do not resolve collisions
	};

	inline PhysicalProperties operator|(PhysicalProperties a, PhysicalProperties b)
	{
		return static_cast<PhysicalProperties>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline PhysicalProperties& operator |=(PhysicalProperties& a, PhysicalProperties b)
	{
		return a = a | b;
	}

	inline PhysicalProperties operator^(PhysicalProperties a, PhysicalProperties b)
	{
		return static_cast<PhysicalProperties>(static_cast<int>(a) ^ static_cast<int>(b));
	}

	inline PhysicalProperties operator^=(PhysicalProperties& a, PhysicalProperties b)
	{
		return a = a ^ b;
	}
	// =======================================================================================

	class RADIANT_API Physics {
	private:
		struct Impl;
		static Impl* m_impl;

	public:
		/*
			Creates a new instance of the Physics engine API.
		*/
		static void Initialize();

		/*
			Deconstructs the current instance of the Physics engine API.
		*/
		static void Destroy();

		/*
			Updates the physical world, moving objects according to their translation vectors, 
			detecting collisions, and resolves them according to rules. Creates a message queue
			to be used by game objects for final update procedures.
		*/
		static void OnUpdate(const float deltaTime);

		/*
			Creates a new self-contained realm to add physical objects to.
		*/
		static RealmID CreateRealm();
		
		/*
			Activates a realm, meaning that the objects in this realm will be updates
			each update cycle.
		*/
		static void ActivateRealm(const RealmID realm);

		/*
			Deactivates an activated realm from the update cycle.
		*/
		static void DeactivateRealm(const RealmID realm);

		/*
			Returns true if a realm is regisrted with the given realmID
		*/
		static bool RealmExists(const RealmID realm);

		/*
			Adds an entity to the provided realm for this session. Returns
			false if realm does not exist.
		*/
		static bool AddEntityToRealm(const RealmID realm, const Entity entity);


		friend class PhysicsSystem;
private:
		
	};
}