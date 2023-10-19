#include "pch.h"
#include "Component.h"
#include "Logging/Log.h"

namespace rdt::core {
	Component::Component()
	{
	}
	Component::~Component()
	{
	}
	void Component::OnMessage(Message msg)
	{
		RDT_CORE_WARN("Core Component - OnMessage Called!");
	}
}
