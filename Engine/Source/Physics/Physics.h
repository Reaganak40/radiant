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
			Adds an object to the physical world. Returns its UUID for future lookups. 
		*/
		static UniqueID CreateObject(const Polygon& nPolygon) { return m_instance->CreateObjectImpl(nPolygon); }

		/*
			Gets a const reference to the polygon indiciated by the Unique identifier.
		*/
		static const Polygon& GetPolygon(UniqueID UUID) { return m_instance->m_objects.at(UUID).m_polygon; }


	private:
		UniqueID CreateObjectImpl(const Polygon& nPolygon);
	};
}