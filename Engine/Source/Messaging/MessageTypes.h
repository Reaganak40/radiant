#pragma once
#include "Utils/UniqueID.h"
#include "GameObject/GameObject.h"

namespace rdt {

	enum DefaultMessage {
		MT_Collision = 0xdead,
		MT_RequestGameObjectPtr,
		MT_SendGameObjectPtr,
	};

	struct CollisionData {
		UniqueID source;

		CollisionData(UniqueID nSource)
			: source(nSource) {}
	};

	struct GameObjectPtrData {
		GameObject* ptr;

		GameObjectPtrData(GameObject* nPtr)
			: ptr(nPtr) {}
	};
}