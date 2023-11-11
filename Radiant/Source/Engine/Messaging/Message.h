#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	class MessageBus;
	enum LoopPhase;
}

namespace rdt {
	using MessageID = unsigned int;
	using MessageType = unsigned int;

	struct RADIANT_API Message {
		MessageID from;
		MessageID to;
		MessageType type;
		void* data;

		friend class MessageBus;
		friend class Communicator;

		/*
			Gets the game loop phase this message was
			instantiated
		*/
		LoopPhase GetCreationDate() const;

		/*
			Returns true if this message has been marked as handled
		*/
		bool IsMessageHandled() const;
	private:
		LoopPhase createdOn;
		bool handled;

		/*
			Free the memory of data.
		*/
		void Destroy();
	};
}