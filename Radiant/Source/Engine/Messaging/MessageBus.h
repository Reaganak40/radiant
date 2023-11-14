/*******************************************************************
*	Module:  Messaging (core)
*	File:    MessageBus.h
*
*	Author: Reagan Kelley
*
*   The MessageBus is a managing internal, that handles the
*	communication between systems, the recylcing of messages,
*	and ensures the safe handling of data.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	using ChannelID = unsigned int;

	enum LoopPhase;
	class Communicator;
}

// Required Definitions for Struct/Class Members
#include "Message.h"

#define RDT_NULL_MESSAGE_ID 0
#define RDT_MESSAGE_BROADCAST 0
#define RDT_NULL_CHANNEL_ID 0

namespace rdt {

	constexpr MessageID SoundMessengerID = 1;
	using MessageQueue = std::vector<Message>;

	class MessageBus {
	private:
		MessageBus();
		~MessageBus();
		static MessageBus* m_instance;

		LoopPhase m_current_phase;

		MessageID idCounter = 0;
		std::unordered_map<std::string, MessageID> aliasToId;
		
		std::vector<MessageQueue> m_channels;

		bool m_in_poll;
		MessageID currentCommunicator;
		ChannelID pollChannel;
		unsigned int lastMessageIndex;
		std::queue<unsigned int> poll_queue;
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
			Called by a communicator object to register to the message bus
		*/
		static MessageID Register(const std::string& alias = "");

		/*
			Gets the communicator with the given alias's messageID.
		*/
		static MessageID GetCommunicator(const std::string& alias);

		/*
			Polls the messages for the given communicator for the given channel.
		*/
		static void PollMessages(MessageID communicator, ChannelID channel);

		/*
			Gets the next polled message for the communicator. Returns true
			if there is no messages left in the poll.
		*/
		static bool GetNextMessage(MessageID communicator, Message& message);

		/*
			Notfies that the last message has been handled and can be removed
			from the message queue.
		*/
		static void MessageHandled();

		/*
			Ends the current message poll.
		*/
		static void EndPoll();

		/*
			Notifies the game loop of the next phase of the game loop, filtering
			old messages from that phase last cylce.
		*/
		static void SetLoopPhase(LoopPhase nPhase);

		/*
			Adds a new message to the message queue for the given channel.
		*/
		static void AddMessage(ChannelID channel, MessageID from, MessageID to, MessageType type, void* data = nullptr);
	
	private:
		MessageID RegisterImpl(const std::string& alias);
		MessageID GetCommunicatorImpl(const std::string& alias);

		void PollMessagesImpl(MessageID communicator, ChannelID channel);
		bool GetNextMessageImpl(MessageID communicator, Message& message);
		void MessageHandledImpl();
		void EndPollImpl();

		void SetLoopPhaseImpl(LoopPhase nPhase);
		void AddMessageImpl(ChannelID channel, MessageID from, MessageID to, MessageType type, void* data);
	};
}