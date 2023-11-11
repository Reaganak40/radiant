#include "pch.h"
#include "Communicator.h"
#include "MessageBus.h"

rdt::Communicator::Communicator()
{
	m_message_id = RDT_NULL_MESSAGE_ID;
}

void rdt::Communicator::Register(const std::string& alias)
{
	m_message_id = MessageBus::Register(alias);
}

void rdt::Communicator::BeginPoll(ChannelID channel)
{
	MessageBus::PollMessages(m_message_id, channel);
}

void rdt::Communicator::EndPoll()
{
	MessageBus::EndPoll();
}

bool rdt::Communicator::GetNextMessage(Message& message)
{
	return MessageBus::GetNextMessage(m_message_id, message);
}


void rdt::Communicator::MessageHandled()
{
	MessageBus::MessageHandled();
}

void rdt::Communicator::AddMessage(ChannelID channel, MessageType type, void* data)
{
	MessageBus::AddMessage(channel, m_message_id, RDT_MESSAGE_BROADCAST, type, data);
}

void rdt::Communicator::AddMessage(ChannelID channel, MessageID to, MessageType type, void* data)
{
	MessageBus::AddMessage(channel, m_message_id, to, type, data);
}

rdt::MessageID rdt::Communicator::FindCommunicator(const std::string& alias)
{
	return MessageBus::GetCommunicator(alias);
}

