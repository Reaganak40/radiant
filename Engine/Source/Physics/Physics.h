#pragma once
#include "pch.h"
#include "Pobject.h"
#include "Utils/UniqueID.h"
namespace Radiant {
	class Physics {
	private:
		Physics();
		~Physics();
		static Physics* m_instance;

		std::unordered_map<UniqueID, Pobject> m_objects;

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
			Adds an object to the physical world. Returns its UUID for future lookups. 
		*/
		static UniqueID CreateObject(std::shared_ptr<Polygon> polygon) { return m_instance->CreateObjectImpl(polygon); }

		/*
			Sets an existing objects properties in the physical world.
		*/
		static void SetObjectProperties(const UniqueID objectID, const unsigned int nProperties) {
			m_instance->SetObjectPropertiesImpl(objectID, nProperties);
		}

		/*
			Removes an existing objects properties in the physical world.
		*/
		static void RemoveObjectProperties(const UniqueID objectID, const unsigned int rProperties) {
			m_instance->RemoveObjectPropertiesImpl(objectID, rProperties);
		}

		/*
			Returns true if the queried object has any of the specified properties.
		*/
		static bool QueryObjectProperties(const UniqueID objectID, const unsigned int propertyQuery) {
			return m_instance->QueryObjectPropertiesImpl(objectID, propertyQuery);
		}

		/*
			Gets a const reference to the polygon indiciated by the Unique identifier.
		*/
		static const Polygon& GetPolygon(UniqueID objectID) { return *m_instance->m_objects.at(objectID).m_polygon; }

		/*
			Sets the friction magnitude of the specified object. [0 to 1], where 0 is
			no friction, and 1 is maximum friction.
		*/
		static void SetFriction(const UniqueID objectID, const double friction) { m_instance->SetFrictionImpl(objectID, friction); }

		/*
			Sets the accerlation vector to the new provided vector.
		*/
		static void SetAcceleration(UniqueID objectID, const Vec2d& nAcceleration) { m_instance->SetAccelerationImpl(objectID, nAcceleration); }
		
		/*
			Sets the acceleration vector in the x-direction
		*/
		static void SetAccelerationX(UniqueID objectID, const double nX) { m_instance->SetAccelerationXImpl(objectID, nX); }
		
		/*
			Sets the acceleration vector in the y-direction
		*/
		static void SetAccelerationY(UniqueID objectID, const double nY) { m_instance->SetAccelerationYImpl(objectID, nY); }

		/*
			Sets a maximum velocity for the specified object.
		*/
		static void SetMaximumVelocity(UniqueID objectID, const Vec2d& nMaxVelocity) { m_instance->SetMaximumVelocityImpl(objectID, nMaxVelocity); }

		/*
			Sets the current velocity for the spciefied object.
		*/
		static void SetVelocity(const UniqueID objectID, Vec2d velocity) { m_instance->SetVelocityImpl(objectID, velocity); }

		/*
			Manually sets the current position of an object, effectively teleporting it.
		*/
		static void SetPosition(const UniqueID objectID, const Vec2d& nPosition) { m_instance->SetPositionImpl(objectID, nPosition); }
	private:

		void OnUpdateImpl(const float deltaTime);
		void OnEndFrameImpl();

		void SetObjectPropertiesImpl(const UniqueID objectID, const unsigned int nProperties);
		void RemoveObjectPropertiesImpl(const UniqueID objectID, const unsigned int nProperties);
		bool QueryObjectPropertiesImpl(const UniqueID objectID, const unsigned int propertyQuery);

		UniqueID CreateObjectImpl(std::shared_ptr<Polygon> polygon);
		void SetAccelerationImpl(UniqueID UUID, const Vec2d& nAcceleration);
		void SetAccelerationXImpl(UniqueID UUID, const double nX);
		void SetAccelerationYImpl(UniqueID UUID, const double nY);

		void SetVelocityImpl(const UniqueID ObjectID, Vec2d& nVelocity);
		
		void SetMaximumVelocityImpl(UniqueID UUID, const Vec2d& nMaxVelocity);

		void SetFrictionImpl(const UniqueID objectID, const double friction);

		void SetPositionImpl(const UniqueID objectID, const Vec2d& nPosition);
	};
}