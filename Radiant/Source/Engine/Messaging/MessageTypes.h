#pragma once
#include "Utils/UniqueID.h"
#include "GameObject/GameObject.h"
#include "Audio/SoundEngine.h"
#include "Scene/Scene.h"

namespace rdt {

	enum DefaultMessage {
		MT_Collision = 0xdead,
		MT_RequestGameObjectPtr,
		MT_SendGameObjectPtr,
		MT_SoundStopped,

		MT_RequestScenePtr,
		MT_SendScenePtr,
		MT_SceneChanged,
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

	struct SoundStoppedData {
		SoundID sID;

		SoundStoppedData(SoundID nSID)
			: sID (nSID) {}
	};

	struct ScenePtrData {
		Scene* ptr;

		ScenePtrData(Scene* nPtr)
			: ptr(nPtr) {}
	};

	struct SceneChangedData {
		Scene* ptr;

		SceneChangedData(Scene* nPtr)
			: ptr(nPtr) {}
	};
}