#pragma once

// Forward Declarations
namespace rdt {
	using UniqueID = unsigned int;
	typedef unsigned int SoundID;
}

// Required Definitions for Struct/Class Members
#include "Scene/Scene.h"

namespace rdt {

	enum DefaultMessage {
		MT_Collision = 0xdead,
		MT_SoundStopped,

		MT_RequestScenePtr,
		MT_SendScenePtr,

		MT_SceneChanged,
		MT_EndOfDefaultMessages,
	};

	enum CoreMessage {
		CM_OpenPanelRequest = MT_EndOfDefaultMessages + 1,
		CM_ChangeSceneRequest,
	};

	struct CollisionData {
		UniqueID objectID;

		CollisionData(UniqueID nSource)
			: objectID(nSource) {}
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