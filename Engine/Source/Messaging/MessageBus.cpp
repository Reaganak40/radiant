#include "pch.h"
#include "MessageBus.h"

namespace rdt {
	
	MessageBus* MessageBus::m_instance = nullptr;
	MessageID MessageBus::idCounter = 1;

	MessageBus::MessageBus()
	{
	}

	MessageBus::~MessageBus()
	{
	}

	void MessageBus::Initialize()
	{
		Destroy();
		m_instance = new MessageBus;
	}
	void MessageBus::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}
	MessageID MessageBus::Register(const std::string& alias, Messenger* messenger)
	{
		if (alias.empty()) {
			return 0;
		}

		MessageID nID = GetNextMessageID();

		m_instance->m_AliasToId[alias] = nID;
		m_instance->m_IdToAlias[nID] = alias;
		m_instance->m_objects[nID] = messenger;

		return nID;

	}

	std::string MessageBus::GetAlias(MessageID mID)
	{
		if (m_instance->m_IdToAlias.find(mID) == m_instance->m_IdToAlias.end()) {
			return "";
		}

		return m_instance->m_IdToAlias.at(mID);
	}

	const MessageID MessageBus::GetMessageID(const std::string& alias)
	{
		if (m_instance->m_AliasToId.find(alias) == m_instance->m_AliasToId.end()) {
			return 0;
		}

		return m_instance->m_AliasToId.at(alias);
	}

	void MessageBus::AddToQueue(const Message& msg)
	{
		if (m_instance->m_objects.find(msg.from) == m_instance->m_objects.end()) {
			return;
		}

		if (m_instance->m_objects.find(msg.to) == m_instance->m_objects.end()) {
			return;
		}

		m_instance->m_message_queue.push(msg);
	}

	void MessageBus::AddToQueue(const std::string& from, const std::string& to, MessageType type, void* data)
	{
		Message nMsg;

		nMsg.from = GetMessageID(from);
		nMsg.to = GetMessageID(to);
		nMsg.type = type;
		nMsg.data = data;

		AddToQueue(nMsg);
	}

	void MessageBus::AddToQueue(const MessageID from, const MessageID to, MessageType type, void* data)
	{
		Message nMsg;

		nMsg.from = from;
		nMsg.to = to;
		nMsg.type = type;
		nMsg.data = data;

		AddToQueue(nMsg);
	}

	void MessageBus::SendDirectMessage(Message& msg)
	{
		if (m_instance->m_objects.find(msg.from) == m_instance->m_objects.end()) {
			return;
		}

		if (m_instance->m_objects.find(msg.to) == m_instance->m_objects.end()) {
			return;
		}

		m_instance->m_objects[msg.to]->OnMessage(msg);
		msg.Destroy();
	}

	void MessageBus::SendDirectMessage(const std::string& from, const std::string& to, MessageType type, void* data)
	{
		Message nMsg;

		nMsg.from = GetMessageID(from);
		nMsg.to = GetMessageID(to);
		nMsg.type = type;
		nMsg.data = data;

		SendDirectMessage(nMsg);
	}

	void MessageBus::SendDirectMessage(const MessageID from, const MessageID to, MessageType type, void* data)
	{
		Message nMsg;

		nMsg.from = from;
		nMsg.to = to;
		nMsg.type = type;
		nMsg.data = data;

		SendDirectMessage(nMsg);
	}

	MessageID MessageBus::GetNextMessageID()
	{
		return ++idCounter;
	}

	void MessageBus::SendMessagesImpl()
	{
		while (!m_message_queue.empty()) {
			auto& msg = m_message_queue.front();
			m_objects[msg.to]->OnMessage(msg);
			msg.Destroy();
			m_message_queue.pop();
		}
	}

}