#include "pch.h"
#include "CommonComponents.h"
#include "Logging/Log.h"
#include "Graphics/Model.h"
#include "Graphics/Animation.h"
#include "Graphics/Texture/TextureManager.h"
#include "Utils/Utils.h"

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
		model = RDT_NULL_MODEL_ID;
	}
	void Sprite::SetModel(const std::string& modelName)
	{
		model = ModelManager::GetModelID(modelName);
	}
	// ===============================================================================
	rdt::Transform::Transform()
	{
		TRACE_COMPONENT_DATA(Transform, position);
		TRACE_COMPONENT_DATA(Transform, rotation);
		TRACE_COMPONENT_DATA(Transform, scale);

		rotation = 0;
	}
	void Transform::Translate(float deltaTime, const Vec2d velocity)
	{
		position += (deltaTime * velocity);
	}
	// ===============================================================================
	RigidBody2D::RigidBody2D()
	{
		TRACE_COMPONENT_DATA(RigidBody2D, mass);
		TRACE_COMPONENT_DATA(RigidBody2D, velocity);

		mass = 1;
	}
	void RigidBody2D::UpdateVelocity(float deltaTime, Vec2d externalForces)
	{
		velocity += deltaTime * (acceleration + externalForces);

		velocity.x = Utils::ApplyEpsilon(velocity.x);
		velocity.y = Utils::ApplyEpsilon(velocity.y);
	}
	// ===============================================================================
	Renderable::Renderable()
	{
		TRACE_COMPONENT_DATA(Renderable, fillColor);
		TRACE_COMPONENT_DATA(Renderable, layer);
		TRACE_COMPONENT_DATA(Renderable, texture);

		layer = 0;
		fillColor = BLACK;
		texture = RDT_NULL_TEXTURE_ID;
		flipTexture = false;
		atlasProfile = TextureManager::NOT_USING_ATLAS();
	}
	// ================================================================================
	rdt::Animator::Animator()
	{
		animationID = RDT_NULL_ANIMATION_ID;
		currentFrame = 0;
	}
}
