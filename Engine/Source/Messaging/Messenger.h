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
			Send a message through the message bus.
		*/
		void SendMessage(MessageID to, MessageType type, void* data = nullptr);

		/*
			Send a message through the message bus, which will be resolved immediately.
		*/
		void SendDirectMessage(const std::string& to, MessageType type, void* data = nullptr);

		/*
			Send a message through the message bus, which will be resolved immediately.
		*/
		void SendDirectMessage(MessageID to, MessageType type, void* data = nullptr);

		/*
			Send a message through the message bus.
		*/
		void SendMessage(const std::string& to, MessageType type, void* data = nullptr);

		/*
			Called when a message is received through the message bus.
		*/
		virtual void OnMessage(Message msg) = 0;
	};
}