#include "pch.h"
#include "CommonComponents.h"
#include "Logging/Log.h"
#include "Graphics/Model.h"


namespace rdt::core {
	void ComponentTraceTracker::AddDefinition(const char* component, const char* memberName, SupportedTraceType type, size_t offset)
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

	std::unordered_map<std::string, core::TraceData>& ComponentTraceTracker::GetTraceData(const char* ComponentName)
	{
		if (ComponentDefinitions.find(ComponentName) == ComponentDefinitions.end()) {
			RDT_CORE_WARN("Could not find traced data for component '{}'", ComponentName);
			ComponentDefinitions[ComponentName];
		}

		return ComponentDefinitions.at(ComponentName);
	}
}

namespace rdt {
	std::unordered_map<std::string, std::unordered_map<std::string, core::TraceData>> core::ComponentTraceTracker::ComponentDefinitions;

	void ECSComponent::DEFINE_MEMBER(const char* component, const char* memberName, core::SupportedTraceType type, size_t offset)
	{
		core::ComponentTraceTracker::AddDefinition(component, memberName, type, offset);
	}


	// ===============================================================================
	EntityConfig::EntityConfig()
	{
		TRACE_COMPONENT_DATA(EntityConfig, owner);
		TRACE_COMPONENT_DATA(EntityConfig, alias);

		owner = nullptr;
	}
	// ===============================================================================
	Sprite::Sprite()
	{
		TRACE_COMPONENT_DATA(Sprite, polygon);

		polygon = nullptr;
		model = NOT_A_MODEL;
	}
	void Sprite::SetModel(const std::string& modelName)
	{
		model = ModelManager::GetModelID(modelName);
	}
	// ===============================================================================
	RigidBody2D::RigidBody2D()
	{
		TRACE_COMPONENT_DATA(RigidBody2D, mass);

		mass = 1;
	}
	// ===============================================================================
	Renderable::Renderable()
	{
		TRACE_COMPONENT_DATA(Renderable, polygon_color);
		TRACE_COMPONENT_DATA(Renderable, layer);
		TRACE_COMPONENT_DATA(Renderable, texture);

		layer = 0;
		polygon_color = BLACK;
	}
}
