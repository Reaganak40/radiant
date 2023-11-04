#include "pch.h"
#include "CommonComponents.h"
#include "Logging/Log.h"

namespace rdt {
	std::unordered_map<std::string, std::unordered_map<std::string, core::TraceData>> ECSComponent::ComponentDefinitions;

	void ECSComponent::DEFINE_MEMBER(const char* component, const char* memberName, core::SupportedTraceType type, size_t offset)
	{
		if (type == core::SupportedTraceType_NotSupported) {
			RDT_CORE_ERROR("Could not define memmber '{}' of component '{}', could not resolve type.", memberName, component);
			return;
		}
		if (ComponentDefinitions.find(component) == ComponentDefinitions.end()) {
			ComponentDefinitions[component];
		}

		if (ComponentDefinitions.at(component).find(memberName) != ComponentDefinitions.at(component).end()) {
			return;
		}

		ComponentDefinitions.at(component)[memberName] = { type, offset };
	}

	std::unordered_map<std::string, core::TraceData>& ECSComponent::GetTraceData(const char* ComponentName)
	{
		if (ComponentDefinitions.find(ComponentName) == ComponentDefinitions.end()) {
			RDT_CORE_WARN("Could not find traced data for component '{}'", ComponentName);
			ComponentDefinitions[ComponentName];
		}

		return ComponentDefinitions.at(ComponentName);
	}
}