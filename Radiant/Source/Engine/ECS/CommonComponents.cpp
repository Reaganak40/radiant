#include "pch.h"
#include "CommonComponents.h"
#include "Logging/Log.h"
#include "Graphics/Model.h"
#include "Graphics/Animation.h"
#include "Graphics/Texture/TextureManager.h"
#include "Utils/Utils.h"
#include "Physics/Physics.h"
#include "Physics/Collider.h"

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

		ComponentDefinitions.at(component).push_back({ memberName, type, offset });
	}

	std::vector<core::TraceData>& ComponentTraceTracker::GetTraceData(const char* ComponentName)
	{
		if (ComponentDefinitions.find(ComponentName) == ComponentDefinitions.end()) {
			RDT_CORE_WARN("Could not find traced data for component '{}'", ComponentName);
			ComponentDefinitions[ComponentName];
		}

		return ComponentDefinitions.at(ComponentName);
	}
	SupportedTraceType ResolveSupportTypeFromString(const std::string& type)
	{
		if (type == "ColliderID") {
			return SupportedTraceType_colliderID;
		}

		if (type == "ModelID") {
			return SupportedTraceType_modelID;
		}
		return SupportedTraceType_NotSupported;
	}
}

namespace rdt {
	std::unordered_map<std::string, std::vector<core::TraceData>> core::ComponentTraceTracker::ComponentDefinitions;

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
	rdt::DebugComponent::DebugComponent()
	{
		show_collider_hitbox = false;
	}
	// ===============================================================================
	Sprite::Sprite()
	{
		TRACE_COMPONENT_DATA(Sprite, model, ModelID);

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
		TRACE_COMPONENT_DATA(Transform, scale);
		TRACE_COMPONENT_DATA(Transform, rotation);

		rotation = 0;
	}
	void Transform::Translate(float deltaTime, const Vec2d velocity)
	{
		position += (deltaTime * velocity);
		position.x = Utils::ApplyEpsilon(position.x);
		position.y = Utils::ApplyEpsilon(position.y);
	}
	// ===============================================================================
	RigidBody2D::RigidBody2D()
	{
		TRACE_COMPONENT_DATA(RigidBody2D, velocity);
		TRACE_COMPONENT_DATA(RigidBody2D, acceleration);
		TRACE_COMPONENT_DATA(RigidBody2D, use_gravity);
		TRACE_COMPONENT_DATA(RigidBody2D, colliderID, ColliderID);
		TRACE_COMPONENT_DATA(RigidBody2D, mass);

		realmID = RDT_NULL_REALM_ID;
		colliderID = RDT_NULL_COLLIDER_ID;
		physicalProperties = PhysicalProperty_None;

		use_gravity = false;
		mass = 1;
	}
	void RigidBody2D::UpdateVelocity(float deltaTime, Vec2d externalForces)
	{
		velocity += deltaTime * (acceleration + externalForces);

		if (max_velocity.x != 0 && max_velocity.y != 0) {
			if (abs(velocity.x) > max_velocity.x) {
				velocity.x = velocity.x > 0 ? max_velocity.x : -max_velocity.x;
			}
			if (abs(velocity.y) > max_velocity.y) {
				velocity.y = velocity.y > 0 ? max_velocity.y : -max_velocity.y;
			}
		}
	}
	Vec2d RigidBody2D::GetChangeInPosition(const float deltaTime)
	{
		return velocity * deltaTime;
	}
	bool RigidBody2D::HasProperties(PhysicalProperties propertyQuery)
	{
		return bool(physicalProperties & propertyQuery);
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
