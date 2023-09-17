#pragma once

namespace rdt {
	typedef unsigned int MessageID;
	typedef unsigned int MessageType;

	struct Message {
		MessageID from;
		MessageID to;
		MessageType type;
		void* data;

		/*
			Free the memory of data.
		*/
		void Destroy();
	};

}