/*******************************************************************
*	Module:  Messaging (API)
*	File:    Communicator.h
*
*	Author: Reagan Kelley
*
*   This file included the Communicator class, which is a device
*	that allows anything to communicate with the components of the
*	engine.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	using MessageID = unsigned int;
	using MessageType = unsigned int;
	using ChannelID = unsigned int;
}

// Required Definitions for Struct/Class Members
#include "Message.h"

namespace rdt {

	class RADIANT_API Communicator {
	private:
		MessageID m_message_id;
	public:

		Communicator();

		/*
			Registers this communicator with the MessageBus, enabling
			communication with the rest of the engine. May provide
			alias so it will be easier for other communicators to find
			you.
		*/
		void Register(const std::string& alias = "");

		/*
			Polls all messages for this communicator from the given channel.
			Polls the main channel by default.
		*/
		void BeginPoll(ChannelID channel);

		/*
			Notifies that message handling is over for this poll, and the
			communicator can send back unhandled messages
		*/
		void EndPoll();

		/*
			Gets the next message from the poll, setting the reference to it. Returns false
			if no messages left.
		*/
		bool GetNextMessage(Message& message);

		/*
			Notifies that that last message has been handled and does
			not need to be sent back to the message queue.
		*/
		void MessageHandled();

		/*
			Adds a new message to the MessageQueue, this message will be
			sent to everyone (broadcasted)
		*/
		void AddMessage(ChannelID channel, MessageType type, void* data = nullptr);

		/*
			Adds a new message to the MessageQueue, this message will only
			be sent to the communicator with the  provided MessageID 'to'
		*/
		void AddMessage(ChannelID channel, MessageID to, MessageType type, void* data = nullptr);

		/*
			Gets the messageID of the communicator with the given alias. If no
			communicator exists, returns 0.
		*/
		MessageID FindCommunicator(const std::string& alias);
	};
}