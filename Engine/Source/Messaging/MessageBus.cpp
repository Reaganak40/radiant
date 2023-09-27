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

		m_instance->m_messengers_AliasToId[alias] = nID;
		m_instance->m_messengers_IdToAlias[nID] = alias;
		m_instance->m_messengers[nID] = messenger;

		return nID;
	}

	std::string MessageBus::GetAlias(MessageID mID)
	{
		if (m_instance->m_messengers_IdToAlias.find(mID) == m_instance->m_messengers_IdToAlias.end()) {
			return "";
		}

		return m_instance->m_messengers_IdToAlias.at(mID);
	}

	const MessageID MessageBus::GetMessageID(const std::string& alias)
	{
		if (m_instance->m_messengers_AliasToId.find(alias) == m_instance->m_messengers_AliasToId.end()) {
			return 0;
		}

		return m_instance->m_messengers_AliasToId.at(alias);
	}

	void MessageBus::AddToQueue(const Message& msg)
	{
		if (m_instance->m_messengers.find(msg.from) == m_instance->m_messengers.end()) {
			return;
		}

		if (m_instance->m_messengers.find(msg.to) == m_instance->m_messengers.end()) {
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
		if (m_instance->m_messengers.find(msg.from) == m_instance->m_messengers.end()) {
			return;
		}

		if (m_instance->m_messengers.find(msg.to) == m_instance->m_messengers.end()) {
			return;
		}

		m_instance->m_messengers[msg.to]->OnMessage(msg);
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
			m_messengers[msg.to]->OnMessage(msg);
			msg.Destroy();
			m_message_queue.pop();
		}
	}

	MessageID MessageBus::CreateBroadcastImpl(const std::string& alias, Broadcast* broadcast)
	{
		if (alias.empty()) {
			return 0;
		}

		MessageID nID = GetNextMessageID();

		m_broadcasts_AliasToId[alias] = nID;
		m_broadcasts_IdToAlias[nID] = alias;
		m_broadcasts[nID] = broadcast;

		return nID;
	}

	const std::vector<Message>& MessageBus::GetBroadcastImpl(const std::string& alias)
	{
		if (m_broadcasts_AliasToId.find(alias) == m_broadcasts_AliasToId.end()) {
			return {};
		}
		return m_broadcasts.at(m_broadcasts_AliasToId.at(alias))->GetMessages();
	}

	void MessageBus::AddToBroadcastImpl(const MessageID broadcastID, MessageType type, void* data)
	{
		if (m_broadcasts.find(broadcastID) == m_broadcasts.end()) {
			return;
		}

		Message nMsg;
		nMsg.from = broadcastID;
		nMsg.to = broadcastID;
		nMsg.type = type;
		nMsg.data = data;

		m_broadcasts.at(broadcastID)->AddToBroadcast(nMsg);
	}

	void MessageBus::ResetBroadcastsImpl()
	{
		for (auto& [mID, broadcast] : m_broadcasts) {
			broadcast->SwapBuffers();
		}
	}

}