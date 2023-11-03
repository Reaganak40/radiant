#include "pch.h"
#include "OOComponent.h"
#include "Logging/Log.h"

namespace rdt::core {
	OOComponent::OOComponent()
	{
	}
	OOComponent::~OOComponent()
	{
	}
	void OOComponent::OnMessage(Message msg)
	{
		RDT_CORE_WARN("Core Component - OnMessage Called!");
	}
}
