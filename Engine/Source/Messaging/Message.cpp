#include "pch.h"
#include "Message.h"

void rdt::Message::Destroy()
{
	delete data;
	data = nullptr;
}
