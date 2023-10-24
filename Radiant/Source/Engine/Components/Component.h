#pragma once

#include "Core.h"
#include "Messaging/Messenger.h"
#include "GameState.h"

namespace rdt::core {

	/*
		Core Components make up that child class instances that define
		the structure of the Game (i.e, GameObjects, Layers, Scenes)
	*/
	class RADIANT_API Component : public Messenger, public GameState {
	private:
	public:
		Component();
		~Component();

		virtual void OnMessage(Message msg);
	};
}