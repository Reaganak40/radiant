#include "pch.h"
#include "MessageBus.h"
#include "Logging/Log.h"

namespace rdt {

	MessageBus* MessageBus::m_instance = nullptr;

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

	MessageID MessageBus::Register(const std::string& alias)
	{
		return m_instance->Register(alias);
	}

	MessageID MessageBus::GetCommunicator(const std::string& alias)
	{
		return MessageID();
	}

	void MessageBus::PollMessages(MessageID communicator, ChannelID channel)
	{
		m_instance->PollMessagesImpl(communicator, channel);
	}

	bool MessageBus::GetNextMessage(MessageID communicator, Message& message)
	{
		return m_instance->GetNextMessageImpl(communicator, message);
	}

	void MessageBus::MessageHandled()
	{
		m_instance->MessageHandledImpl();
	}

	void MessageBus::EndPoll()
	{

	}

	void MessageBus::SetLoopPhase(LoopPhase nPhase)
	{
		m_instance->SetLoopPhaseImpl(nPhase);
	}

	void MessageBus::AddMessage(ChannelID channel, MessageID from, MessageID to, MessageType type, void* data)
	{
		
	}

	MessageID MessageBus::RegisterImpl(const std::string& alias)
	{
		MessageID nID = ++idCounter;
		if (!alias.empty()) {
			aliasToId[alias] = nID;
		}

		return nID;
	}

	MessageID MessageBus::GetCommunicatorImpl(const std::string& alias)
	{
		if (aliasToId.find(alias) == aliasToId.end()) {
			return RDT_NULL_MESSAGE_ID;
		}

		return aliasToId.at(alias);
	}

	void MessageBus::PollMessagesImpl(MessageID communicator, ChannelID channel)
	{
		if (m_in_poll) {
			RDT_CORE_ERROR("MessageBus - Trying to poll messages but already in a poll!");
			return;
		}

		if (m_channels.size() >= channel) {
			RDT_CORE_ERROR("MessageBus - Trying to poll messages from unregisrted channel [{}]", channel);
			return;
		}

		if (communicator == RDT_NULL_MESSAGE_ID) {
			RDT_CORE_ERROR("MessageBus - Trying to poll messages from unregistered communicator");
			return;
		}

		// clear the poll queue
		std::queue<unsigned int> empty;
		std::swap(poll_queue, empty);

		auto& messageQueue = m_channels[channel];

		for (unsigned int i = 0; i < messageQueue.size(); i++) {

			if (messageQueue[i].handled) {
				continue;
			}

			if (messageQueue[i].to == communicator || messageQueue[i].to == RDT_MESSAGE_BROADCAST) {
				poll_queue.push(i);
			}
		}

		m_in_poll = true;
		currentCommunicator = communicator;
		pollChannel = channel;
	}

	bool MessageBus::GetNextMessageImpl(MessageID communicator, Message& message)
	{
		if (!m_in_poll) {
			RDT_CORE_ERROR("MessageBus - Communicator [{}] tried to get next message, but no poll available", communicator);
			return false;
		}
		if (communicator != currentCommunicator) {
			RDT_CORE_ERROR("MessageBus - Communicator [{}] tried to get next message but is not the current communicator", communicator);
			return false;
		}

		if (poll_queue.size() == 0) {
			return false;
		}

		unsigned int nMessageIndex = poll_queue.front();
		poll_queue.pop();

		message = m_channels[pollChannel][nMessageIndex];
		lastMessageIndex = nMessageIndex;
		return true;
	}

	void MessageBus::MessageHandledImpl()
	{
		if (!m_in_poll) {
			RDT_CORE_ERROR("MessageBus - Tried to handle message but not in poll.");
			return;
		}

		m_channels[pollChannel][lastMessageIndex].handled = true;
	}

	void MessageBus::EndPollImpl()
	{
		m_in_poll = false;
		currentCommunicator = RDT_NULL_MESSAGE_ID;
	}

	void MessageBus::SetLoopPhaseImpl(LoopPhase nPhase)
	{
		m_current_phase = nPhase;

		for (auto& channel : m_channels) {
			// Remove handled messages and old messages
			channel.erase(std::remove_if(channel.begin(), channel.end(), [&](const Message& msg) {
				return msg.GetCreationDate() == nPhase || msg.IsMessageHandled();
				}), channel.end());
		}
	}

	void MessageBus::AddMessageImpl(ChannelID channel, MessageID from, MessageID to, MessageType type, void* data)
	{
		if (channel >= m_channels.size()) {
			RDT_CORE_WARN("MessageBus - Tried to add message to unregistered channel: [{}]", channel);
			return;
		}

		if (from == RDT_NULL_MESSAGE_ID) {
			RDT_CORE_WARN("MessageBus - Tried to add message to unregistered communicator.");
			return;
		}

		m_channels[channel].push_back({});
		auto& nMsg = m_channels[channel].back();

		nMsg.from = from;
		nMsg.to = to;
		nMsg.type = type;
		nMsg.data = data;
		nMsg.createdOn = m_current_phase;
		nMsg.handled = false;
	}

}