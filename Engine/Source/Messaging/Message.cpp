#include "pch.h"
#include "Message.h"

void rdt::Message::Destroy()
{
	if (data != nullptr) {
		delete data;
		data = nullptr;
	}
}
