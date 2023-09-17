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

	void Messenger::RegisterToMessageBus(std::string& alias)
	{
		MessageBus::Register(alias, this);
	}
}
