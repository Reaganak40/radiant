#include "pch.h"
#include "Broadcast.h"

namespace rdt {
	Broadcast::Broadcast()
	{
		m_current_buffer = 0;
	}
	Broadcast::~Broadcast()
	{
		SwapBuffers();
		SwapBuffers();
	}
	const std::vector<Message>& Broadcast::GetMessages()
	{
		m_mutex.lock();
		unsigned int index = m_current_buffer == 0 ? 1 : 0;
		m_mutex.unlock();

		return m_messages[index];

	}
	void Broadcast::AddToBroadcast(const Message& nMessage)
	{
		m_mutex.lock();
		m_messages[m_current_buffer].push_back(nMessage);
		m_mutex.unlock();
	}

	void Broadcast::SwapBuffers()
	{
		m_mutex.lock();

		m_current_buffer = m_current_buffer == 0 ? 1 : 0;
		for (auto& message : m_messages[m_current_buffer]) {
			message.Destroy();
		}
		m_messages[m_current_buffer].clear();

		m_mutex.unlock();

	}
}
