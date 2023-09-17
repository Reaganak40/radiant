#pragma once
#include "Message.h"
#include "Messenger.h"
#include "Utils/UniqueID.h"

namespace rdt {
	
	enum DefaultMessage {
		MT_Collision = 0xdead
	};

	struct CollisionData {
		UniqueID source;

		CollisionData(UniqueID nSource)
			: source(nSource) {}
	};

	class MessageBus {
	private:
		MessageBus();
		~MessageBus();
		static MessageBus* m_instance;
		static MessageID idCounter;

		std::unordered_map<std::string, MessageID> m_AliasToId;
		std::unordered_map<MessageID, std::string> m_IdToAlias;

		std::unordered_map<MessageID, Messenger*> m_objects;

		std::queue<Message> m_message_queue;

	public:
		/*
			Create a new singleton instance of the Message Bus
		*/
		static void Initialize();

		/*
			Destroys the current singleton instance of the Message Bus.
		*/
		static void Destroy();

		/*
			Runs all messages in the message queue, sending them all to their
			receivers.
		*/
		static void SendMessages() { m_instance->SendMessagesImpl(); }

		/*
			Registers an object to the message bus, by providing a unique alias and a pointer to
			its OnMessage function. Returns the unique messageID for this registered object.
		*/
		static MessageID Register(const std::string& alias, Messenger* messenger);

		/*
			Gets the unique alias from the provided MessageID, returns empty string if it
			does not exist.
		*/
		static std::string GetAlias(MessageID mID);

		/*
			Gets the messageID belonging to the provided alias. Returns 0 if it does not exist.
		*/
		static const MessageID GetMessageID(const std::string& alias);

		/*
			Adds a new message to the Message Queue.
		*/
		static void AddToQueue(const Message& msg);

		/*
			Adds a new message to the Message Queue.
		*/
		static void AddToQueue(const std::string& from, const std::string& to, MessageType type, void* data);

		/*
			Adds a new message to the Message Queue.
		*/
		static void AddToQueue(const MessageID from, const MessageID to, MessageType type, void* data);

	private:
		static MessageID GetNextMessageID();
		void SendMessagesImpl();
	};
}