#pragma once

#include "Polygon/Polygon.h"
#include "Utils/UniqueID.h"
#include "Pobject.h"
#include "Messaging/MessageBus.h"

namespace rdt::core {

	class Realm {
	private:
		UniqueID m_ID;
		std::unordered_map<UniqueID, Pobject> m_objects;
		std::unordered_map<MessageID, UniqueID> m_object_mIDs;

	public:
		Realm();
		~Realm();

		void OnUpdate(const float deltaTime);
		void OnEndFrame();
		const UniqueID GetUUID() { return m_ID; }

		const UniqueID CreatePhysicsObject(std::shared_ptr<Polygon> polygon, const MessageID messageID);
		Pobject* GetPhysicsObject(const UniqueID UUID);

		friend class Physics;
	};
}