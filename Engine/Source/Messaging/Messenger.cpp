#include "pch.h"
#include "Messenger.h"
#include "MessageBus.h"

namespace rdt {

	Messenger::Messenger()
		: m_messageID(0)
	{
	}
	Messenger::~Messenger()
	{
	}

	void Messenger::RegisterToMessageBus(const std::string& alias)
	{
		m_messageID = MessageBus::Register(alias, this);
	}
}
