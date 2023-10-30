#pragma once

#include "Core.h"
#include "Messaging/Messenger.h"
#include "GameState.h"

namespace rdt::core {

	/*
		Core Object-Oriented Components make up that child class instances that define
		the structure of the Game (i.e, GameObjects, Layers, Scenes)
	*/
	class RADIANT_API OOComponent : public Messenger, public GameState {
	private:
	public:
		OOComponent();
		~OOComponent();

		virtual void OnMessage(Message msg);
	};
}