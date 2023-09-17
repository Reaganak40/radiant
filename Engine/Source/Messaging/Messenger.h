#pragma once
#include "Message.h"

namespace rdt {
	class Messenger {
	private:
		MessageID m_messageID;
	protected:

		/*
			Registers this game object with the Message Bus, the core messaging system in Radiant.
			This will set the MessageID to a unique identifer which can be used to send and receive
			messages.
		*/
		void RegisterToMessageBus(const std::string& alias);

	public:
		Messenger();
		~Messenger();

		/*
			Gets the MessageID for this object, which can be used to send and receive messages.
		*/
		const MessageID GetMessageID() { return m_messageID; }

		/*
			Called when a message is received through the message bus.
		*/
		virtual void OnMessage(Message msg) = 0;
	};
}