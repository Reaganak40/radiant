#pragma once
#include "Message.h"

#ifdef SendMessage
#undef SendMessage
#endif

namespace rdt {
	class Messenger {
	private:
		MessageID m_messageID;
		std::string m_name;
	protected:

		/*
			Registers this game object with the Message Bus, the core messaging system in Radiant.
			This will set the MessageID to a unique identifer which can be used to send and receive
			messages. Also sets the internal name for this messanger class. 
		*/
		bool RegisterToMessageBus(const std::string& alias);

	public:
		Messenger();
		~Messenger();

		/*
			Gets the MessageID for this object, which can be used to send and receive messages.
		*/
		const MessageID GetMessageID() { return m_messageID; }

		/*
			Gets the registered name for this object (i.e., alias)
		*/
		std::string GetName() { return m_name; }

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

		friend class MessageBus;
	private:
		void SetMessageID(MessageID nID) { m_messageID = nID; }
	};
}