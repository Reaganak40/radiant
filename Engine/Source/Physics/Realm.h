#pragma once

#include "Polygon/Polygon.h"
#include "Utils/UniqueID.h"
#include "Pobject.h"

namespace rdt {

	class Realm {
	private:
		UniqueID m_ID;
		std::unordered_map<UniqueID, Pobject> m_objects;

	public:
		Realm();
		~Realm();

		void OnUpdate(const float deltaTime);
		void OnEndFrame();
		const UniqueID GetUUID() { return m_ID; }

		const UniqueID CreatePhysicsObject(std::shared_ptr<Polygon> polygon);
		Pobject* GetPhysicsObject(const UniqueID UUID);

		friend class Physics;
	};
}