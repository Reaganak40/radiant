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
		static UniqueID CreateObject(const Polygon& nPolygon) { return m_instance->CreateObjectImpl(nPolygon); }

		/*
			Gets a const reference to the polygon indiciated by the Unique identifier.
		*/
		static const Polygon& GetPolygon(UniqueID UUID) { return m_instance->m_objects.at(UUID).m_polygon; }


		/*
			Sets the accerlation vector to the new provided vector.
		*/
		static void SetAcceleration(UniqueID UUID, const Vec2d& nAcceleration) { m_instance->SetAccelerationImpl(UUID, nAcceleration); }
		
		/*
			Sets the acceleration vector in the x-direction
		*/
		static void SetAccelerationX(UniqueID UUID, const double nX) { m_instance->SetAccelerationXImpl(UUID, nX); }
		
		/*
			Sets the acceleration vector in the y-direction
		*/
		static void SetAccelerationY(UniqueID UUID, const double nY) { m_instance->SetAccelerationYImpl(UUID, nY); }

		/*
			Sets a maximum velocity for the specified object.
		*/
		static void SetMaximumVelocity(UniqueID UUID, const Vec2d& nMaxVelocity) { m_instance->SetMaximumVelocityImpl(UUID, nMaxVelocity); }


	private:

		void OnUpdateImpl(const float deltaTime);
		void OnEndFrameImpl();


		UniqueID CreateObjectImpl(const Polygon& nPolygon);
		void SetAccelerationImpl(UniqueID UUID, const Vec2d& nAcceleration);
		void SetAccelerationXImpl(UniqueID UUID, const double nX);
		void SetAccelerationYImpl(UniqueID UUID, const double nY);

		
		void SetMaximumVelocityImpl(UniqueID UUID, const Vec2d& nMaxVelocity);

	};
}