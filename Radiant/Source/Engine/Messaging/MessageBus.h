#pragma once
#include "Core.h"
#include "Message.h"
#include "Messenger.h"
#include "Broadcast.h"

namespace rdt {

	constexpr MessageID SoundMessengerID = 1;

	class RADIANT_API MessageBus {
	private:
		MessageBus();
		~MessageBus();
		static MessageBus* m_instance;
		static MessageID idCounter;

		std::queue<Message> m_message_queue;

		/***********************************************************
		*
		*    	  Messenger Dictionary and Register Tables
		*
		************************************************************/
		std::unordered_map<std::string, MessageID> m_messengers_AliasToId;
		std::unordered_map<MessageID, std::string> m_messengers_IdToAlias;
		std::unordered_map<MessageID, Messenger*> m_messengers;

		/***********************************************************
		*
		*    	  Broadcast Dictionary and Register Tables
		*
		************************************************************/
		std::unordered_map<std::string, MessageID> m_broadcasts_AliasToId;
		std::unordered_map<MessageID, std::string> m_broadcasts_IdToAlias;
		std::unordered_map<MessageID, Broadcast*> m_broadcasts;

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
			Removes all old messages from the registered broadcasts and replaces
			them with new ones.
		*/
		static void ResetBroadcasts() { m_instance->ResetBroadcastsImpl(); }

		/*
			Registers an object to the message bus, by providing a unique alias and a pointer to
			its OnMessage function. Returns the unique messageID for this registered object.
		*/
		static MessageID Register(const std::string& alias, Messenger* messenger);

		/*
			Creates a new broadcast instance under the provided alias. The owner of the broadcast
			should be the only entity that can add broadcast messages, but all entities should be
			able to see the broadcast messages at any time.
		*/
		static MessageID CreateBroadcast(const std::string& alias, Broadcast* broadcast) { return m_instance->CreateBroadcastImpl(alias, broadcast); }

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

		/*
			Sends a message directly to the receiver, calling its OnMessage immediately.
		*/
		static void SendDirectMessage(Message& msg);
		
		/*
			Sends a message directly to the receiver, calling its OnMessage immediately.
		*/
		static void SendDirectMessage(const std::string& from, const std::string& to, MessageType type, void* data);

		/*
			Sends a message directly to the receiver, calling its OnMessage immediately.
		*/
		static void SendDirectMessage(const MessageID from, const MessageID to, MessageType type, void* data);

		static void AddToBroadcast(const MessageID broadcastID, MessageType type, void* data) { m_instance->AddToBroadcastImpl(broadcastID, type, data); }

		/*
			Returns the current display of messages from a registered broadcast.
		*/
		static const std::vector<Message>& GetBroadcast(const std::string& alias) { return m_instance->GetBroadcastImpl(alias); }

	private:
		static MessageID GetNextMessageID();
		void SendMessagesImpl();
		MessageID CreateBroadcastImpl(const std::string& alias, Broadcast* broadcast);
		const std::vector<Message>& GetBroadcastImpl(const std::string& alias);
		void AddToBroadcastImpl(const MessageID broadcastID, MessageType type, void* data);
		void ResetBroadcastsImpl();
	};
}