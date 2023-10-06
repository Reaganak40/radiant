#pragma once
#include "Core.h"

namespace rdt {
	typedef RADIANT_API unsigned int MessageID;
	typedef RADIANT_API unsigned int MessageType;

	struct RADIANT_API Message {
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