#include "pch.h"
#include "Messenger.h"
#include "MessageBus.h"

namespace rdt {

	Messenger::Messenger()
		: m_messageID(0)
	{
		m_name = "NoName";
	}
	Messenger::~Messenger()
	{
	}

	void Messenger::SendMessage(MessageID to, MessageType type, void* data)
	{
		MessageBus::AddToQueue(m_messageID, to, type, data);
	}

	void Messenger::SendDirectMessage(const std::string& to, MessageType type, void* data)
	{
		MessageBus::SendDirectMessage(m_messageID, MessageBus::GetMessageID(to), type, data);
	}

	void Messenger::SendDirectMessage(MessageID to, MessageType type, void* data)
	{
		MessageBus::SendDirectMessage(m_messageID, to, type, data);
	}

	void Messenger::SendMessage(const std::string& to, MessageType type, void* data)
	{
		MessageBus::AddToQueue(m_messageID, MessageBus::GetMessageID(to), type, data);
	}

	bool Messenger::RegisterToMessageBus(const std::string& alias)
	{
		m_messageID = MessageBus::Register(alias, this);

		if (!m_messageID) {
			return false;
		}

		m_name = alias;
		return true;
	}
}
