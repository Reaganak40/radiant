#include "pch.h"
#include "Messenger.h"
#include "MessageBus.h"

namespace rdt {

	struct Messenger::Impl {
		MessageID m_messageID;
		std::string m_name;

		Impl()
			: m_messageID(0)
		{
			m_name = "NoName";
		}

		~Impl()
		{
		}
	};

	void Messenger::RemoveFromMessageBus()
	{
		MessageBus::RemoveMessenger(m_impl->m_messageID);
		m_impl->m_name = "NoName";
		m_impl->m_messageID = 0;
	}

	Messenger::Messenger()
		: m_impl(new Messenger::Impl)
	{
	}
	Messenger::~Messenger()
	{
		if (m_impl->m_messageID != 0) {
			RemoveFromMessageBus();
		}
		delete m_impl;
	}

	const MessageID Messenger::GetMessageID()
	{
		return m_impl->m_messageID;
	}

	std::string Messenger::GetName()
	{
		return m_impl->m_name;
	}

	void Messenger::SendMessage(MessageID to, MessageType type, void* data)
	{
		MessageBus::AddToQueue(m_impl->m_messageID, to, type, data);
	}

	void Messenger::SendDirectMessage(const std::string& to, MessageType type, void* data)
	{
		MessageBus::SendDirectMessage(m_impl->m_messageID, MessageBus::GetMessageID(to), type, data);
	}

	void Messenger::SendDirectMessage(MessageID to, MessageType type, void* data)
	{
		MessageBus::SendDirectMessage(m_impl->m_messageID, to, type, data);
	}

	void Messenger::SendMessage(const std::string& to, MessageType type, void* data)
	{
		MessageBus::AddToQueue(m_impl->m_messageID, MessageBus::GetMessageID(to), type, data);
	}

	void Messenger::SetMessageID(MessageID nID)
	{
		m_impl->m_messageID = nID;
	}

	bool Messenger::RegisterToMessageBus(const std::string& alias)
	{
		m_impl->m_messageID = MessageBus::Register(alias, this);

		if (!m_impl->m_messageID) {
			return false;
		}

		m_impl->m_name = alias;
		return true;
	}
}
