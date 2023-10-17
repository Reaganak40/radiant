#include "pch.h"
#include "MessageBus.h"
#include "Logging/Log.h"

namespace rdt {
	
	struct MessageBus::Impl {
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

		MessageID Register(const std::string& alias, Messenger* messenger)
		{
			if (alias.empty()) {
				return 0;
			}

			if (m_messengers_AliasToId.find(alias) != m_messengers_AliasToId.end()) {
				RDT_CORE_WARN("MessageBus: Could not register '{}' (already exists)", alias);
				return 0;
			}

			MessageID nID = GetNextMessageID();

			m_messengers_AliasToId[alias] = nID;
			m_messengers_IdToAlias[nID] = alias;
			m_messengers[nID] = messenger;

			RDT_CORE_TRACE("MessageBus: Registered new object '{}' to [mID: {}]", alias, nID);

			return nID;
		}

		std::string GetAlias(MessageID mID)
		{
			if (m_messengers_IdToAlias.find(mID) == m_messengers_IdToAlias.end()) {
				return "";
			}

			return m_messengers_IdToAlias.at(mID);
		}

		const MessageID GetMessageID(const std::string& alias)
		{
			if (m_messengers_AliasToId.find(alias) == m_messengers_AliasToId.end()) {
				return 0;
			}

			return m_messengers_AliasToId.at(alias);
		}

		void AddToQueue(Message& msg)
		{
			if (msg.from == 0) {
				RDT_CORE_WARN("Message sent from unregister object. Message dropped.", msg.from);
				DropMessage(msg);
				return;
			}

			if (m_messengers.find(msg.from) == m_messengers.end()) {
				RDT_CORE_WARN("Message sent from unknown object [mID: {}]. Message dropped.", msg.from);
				DropMessage(msg);
				return;
			}

			if (m_messengers.find(msg.to) == m_messengers.end()) {
				RDT_CORE_WARN("Message sent to unknown object [mID: {}]. Message dropped.", msg.to);
				DropMessage(msg);
				return;
			}

			m_message_queue.push(msg);
		}

		void SendDirectMessage(Message& msg)
		{
			if (m_messengers.find(msg.from) == m_messengers.end()) {
				return;
			}

			if (m_messengers.find(msg.to) == m_messengers.end()) {
				return;
			}

			m_messengers[msg.to]->OnMessage(msg);
			msg.Destroy();
		}

		MessageID GetNextMessageID()
		{
			return ++idCounter;
		}

		void SendMessages()
		{
			while (!m_message_queue.empty()) {
				auto& msg = m_message_queue.front();
				m_messengers[msg.to]->OnMessage(msg);
				msg.Destroy();
				m_message_queue.pop();
			}
		}

		MessageID CreateBroadcast(const std::string& alias, Broadcast* broadcast)
		{
			if (alias.empty()) {
				return 0;
			}

			if (m_broadcasts_AliasToId.find(alias) != m_broadcasts_AliasToId.end()) {
				RDT_CORE_WARN("MessageBus: Could not register broadcast '{}' (already exists)", alias);
				return 0;
			}

			MessageID nID = GetNextMessageID();

			m_broadcasts_AliasToId[alias] = nID;
			m_broadcasts_IdToAlias[nID] = alias;
			m_broadcasts[nID] = broadcast;

			return nID;
		}

		const std::vector<Message>* GetBroadcast(const std::string& alias)
		{
			if (m_broadcasts_AliasToId.find(alias) == m_broadcasts_AliasToId.end()) {
				return nullptr;
			}
			return m_broadcasts.at(m_broadcasts_AliasToId.at(alias))->GetMessages();
		}

		void AddToBroadcast(const MessageID broadcastID, MessageType type, void* data)
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

		void ResetBroadcasts()
		{
			for (auto& [mID, broadcast] : m_broadcasts) {
				broadcast->SwapBuffers();
			}
		}

		void DropMessage(Message& msg)
		{
			msg.Destroy();
		}
	};

	// ==============================================================================

	MessageBus* MessageBus::m_instance = nullptr;
	MessageID MessageBus::Impl::idCounter = 1;

	MessageBus::MessageBus()
		: m_impl(new MessageBus::Impl)
	{
	}

	MessageBus::~MessageBus()
	{
		delete m_impl;
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
		return m_instance->m_impl->Register(alias, messenger);
	}

	std::string MessageBus::GetAlias(MessageID mID)
	{
		return m_instance->m_impl->GetAlias(mID);
	}

	const MessageID MessageBus::GetMessageID(const std::string& alias)
	{
		return m_instance->m_impl->GetMessageID(alias);
	}

	void MessageBus::AddToQueue(Message& msg)
	{
		m_instance->m_impl->AddToQueue(msg);
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
		m_instance->m_impl->SendDirectMessage(msg);
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
		return m_instance->m_impl->GetNextMessageID();
	}

	void MessageBus::SendMessagesImpl()
	{
		m_impl->SendMessages();
	}

	MessageID MessageBus::CreateBroadcastImpl(const std::string& alias, Broadcast* broadcast)
	{
		return m_impl->CreateBroadcast(alias, broadcast);
	}

	const std::vector<Message>* MessageBus::GetBroadcastImpl(const std::string& alias)
	{
		return m_impl->GetBroadcast(alias);
	}

	void MessageBus::AddToBroadcastImpl(const MessageID broadcastID, MessageType type, void* data)
	{
		m_impl->AddToBroadcast(broadcastID, type, data);
	}

	void MessageBus::ResetBroadcastsImpl()
	{
		m_impl->ResetBroadcasts();
	}

}