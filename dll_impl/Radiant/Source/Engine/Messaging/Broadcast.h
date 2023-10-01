#pragma once
#include "Message.h"

namespace rdt {
	class Broadcast {
	private:
		std::vector<Message> m_messages[2];
		unsigned int m_current_buffer;

		std::mutex m_mutex;

	public:
		Broadcast();
		~Broadcast();

		/*
			Returns a list of all messages being displayed by this broadcast.
		*/
		const std::vector<Message>& GetMessages();

		friend class MessageBus;
	private:
		/*
			Adds a new message to the broadcast.
		*/
		void AddToBroadcast(const Message& nMessage);

		/*
			Clears the broadcast of old messages and shows new ones by
			swapping the buffers.
		*/
		void SwapBuffers();
	};
}