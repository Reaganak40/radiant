#include "pch.h"
#include "Message.h"
#include "Application.h"

rdt::LoopPhase rdt::Message::GetCreationDate() const
{
	return createdOn;
}

bool rdt::Message::IsMessageHandled() const
{
	return handled;
}

void rdt::Message::Destroy()
{
	if (data != nullptr) {
		delete data;
		data = nullptr;
	}
}
