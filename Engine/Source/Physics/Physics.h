#pragma once
#include "pch.h"
#include "Pobject.h"
#include "Utils/UniqueID.h"
#include "Realm.h"
#include "Messaging/MessageBus.h"

namespace rdt {
	class Physics {
	private:
		Physics();
		~Physics();
		static Physics* m_instance;

		std::unordered_map<UniqueID, Realm*> m_realms;
		std::set<UniqueID> m_active_realms;

		Polygon m_bad_poly;

	public:
		static Physics* GetInstance();

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
		static void OnUpdate(const float deltaTime) { m_instance->OnUpdateImpl(deltaTime); }

		/*
			Performs end of frame routines for the physics API.
		*/
		static void OnEndFrame() { m_instance->OnEndFrameImpl(); }


		/*
			Creates a new self-contained realm to add physical objects to.
		*/

		static UniqueID CreateRealm() { return m_instance->CreateRealmImpl(); }
		
		/*
			Activates a realm, meaning that the objects in this realm will be updates
			each update cycle.
		*/
		static void ActivateRealm(const UniqueID realmID) { m_instance->ActivateRealmImpl(realmID); }

		/*
			Deactivates an activated realm from the update cycle.
		*/
		static void DeactivateRealm(const UniqueID realmID) { m_instance->DeactivateRealmImpl(realmID); }

		/*
			Adds an object to the physical world. Returns its UUID for future lookups. 
		*/
		static UniqueID CreateObject(const UniqueID realmID, const MessageID messageID, std::shared_ptr<Polygon> polygon) { return m_instance->CreateObjectImpl(realmID, messageID, polygon); }

		/*
			Sets an existing objects properties in the physical world.
		*/
		static void SetObjectProperties(const UniqueID realmID, const UniqueID objectID, const unsigned int nProperties) {
			m_instance->SetObjectPropertiesImpl(realmID, objectID, nProperties);
		}

		/*
			Removes an existing objects properties in the physical world.
		*/
		static void RemoveObjectProperties(const UniqueID realmID, const UniqueID objectID, const unsigned int rProperties) {
			m_instance->RemoveObjectPropertiesImpl(realmID, objectID, rProperties);
		}

		/*
			Returns true if the queried object has any of the specified properties.
		*/
		static bool QueryObjectProperties(const UniqueID realmID, const UniqueID objectID, const unsigned int propertyQuery) {
			return m_instance->QueryObjectPropertiesImpl(realmID, objectID, propertyQuery);
		}

		/*
			Gets a const reference to the polygon indiciated by the Unique identifier.
		*/
		static const Polygon& GetPolygon(const UniqueID realmID, const UniqueID objectID) { return m_instance->GetPolygonImpl(realmID, objectID); }

		/*
			Adds a tag to the physics object, which will be used to interact with other objects.
		*/
		static void AddPTag(const UniqueID realmID, const UniqueID objectID, const std::string& tagName) { m_instance->AddPTagImpl(tagName, realmID, objectID); }
		
		/*
			Sets the friction magnitude of the specified object. [0 to 1], where 0 is
			no friction, and 1 is maximum friction.
		*/
		static void SetFriction(const UniqueID realmID, const UniqueID objectID, const double friction) { m_instance->SetFrictionImpl(realmID, objectID, friction); }

		/*
			Sets the accerlation vector to the new provided vector.
		*/
		static void SetAcceleration(const UniqueID realmID, const UniqueID objectID, const Vec2d& nAcceleration) { m_instance->SetAccelerationImpl(realmID, objectID, nAcceleration); }
		
		/*
			Sets the acceleration vector in the x-direction
		*/
		static void SetAccelerationX(const UniqueID realmID, const UniqueID objectID, const double nX) { m_instance->SetAccelerationXImpl(realmID, objectID, nX); }
		
		/*
			Sets the acceleration vector in the y-direction
		*/
		static void SetAccelerationY(const UniqueID realmID, const UniqueID objectID, const double nY) { m_instance->SetAccelerationYImpl(realmID, objectID, nY); }

		/*
			Sets a maximum velocity for the specified object.
		*/
		static void SetMaximumVelocity(const UniqueID realmID, const UniqueID objectID, const Vec2d& nMaxVelocity) { m_instance->SetMaximumVelocityImpl(realmID, objectID, nMaxVelocity); }

		/*
			Sets the current velocity for the spciefied object.
		*/
		static void SetVelocity(const UniqueID realmID, const UniqueID objectID, Vec2d velocity) { m_instance->SetVelocityImpl(realmID, objectID, velocity); }

		/*
			Manually sets the current position of an object, effectively teleporting it.
		*/
		static void SetPosition(const UniqueID realmID, const UniqueID objectID, const Vec2d& nPosition) { m_instance->SetPositionImpl(realmID, objectID, nPosition); }
		
		/*
			Manually sets the current rotation of an object.
		*/
		static void SetRotation(const UniqueID realmID, const UniqueID objectID, const double nRadians) { m_instance->SetRotationImpl(realmID, objectID, nRadians); }

		/*
			Set the hitbox size of the physics object. This is by scale where 1x1 makes the hitbox
			the same as the shape of the polygon, and less than that shrinks the hitbox.
		*/
		static void SetHitBoxSize(const UniqueID realmID, const UniqueID objectID, const Vec2d& nSize) {
			m_instance->SetHitBoxSizeImpl(realmID, objectID, nSize);
		}
		/*
			Gets the current velocity of the queried object.
		*/
		static Vec2d GetVelocity(const UniqueID realmID, const UniqueID objectID) { return m_instance->GetVelocityImpl(realmID, objectID); }
private:

		void OnUpdateImpl(const float deltaTime);
		void OnEndFrameImpl();

		UniqueID CreateRealmImpl();
		void ActivateRealmImpl(const UniqueID realmID);
		void DeactivateRealmImpl(const UniqueID realmID);


		const Polygon& GetPolygonImpl(const UniqueID realmID, const UniqueID objectID);

		void SetObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, const unsigned int nProperties);
		void RemoveObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, const unsigned int nProperties);
		bool QueryObjectPropertiesImpl(const UniqueID realmID, const UniqueID objectID, const unsigned int propertyQuery);

		UniqueID CreateObjectImpl(const UniqueID realmID, const MessageID messageID, std::shared_ptr<Polygon> polygon);
		
		void AddPTagImpl(const std::string& tagName, const UniqueID realmID, const UniqueID objectID);
		void SetAccelerationImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nAcceleration);
		void SetAccelerationXImpl(const UniqueID realmID, const UniqueID objectID, const double nX);
		void SetAccelerationYImpl(const UniqueID realmID, const UniqueID objectID, const double nY);
		void SetVelocityImpl(const UniqueID realmID, const UniqueID objectID, Vec2d& nVelocity);
		void SetMaximumVelocityImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nMaxVelocity);
		void SetFrictionImpl(const UniqueID realmID, const UniqueID objectID, const double friction);
		void SetPositionImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nPosition);
		void SetRotationImpl(const UniqueID realmID, const UniqueID objectID, const double nRadians);
		void SetHitBoxSizeImpl(const UniqueID realmID, const UniqueID objectID, const Vec2d& nSize);

		Vec2d GetVelocityImpl(const UniqueID realmID, const UniqueID objectID);
	};
}