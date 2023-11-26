/*******************************************************************
*	Module:  ECS  
*	File:    CommonComponents.h
*	
*	Author: Reagan Kelley
* 
*   Common component data structs that are used by the engine,
*	systems, and child applications.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	using ModelID = unsigned int;
	using TextureID = unsigned int;   // Unique Identifier for a Texture
	using AnimationID = unsigned int;    // Unique Identifier for an Animation object.
	using AnimationIndex = unsigned int; // Indicates the index of an animation sequence
	using RealmID = unsigned int;
	using ColliderID = unsigned int;

	enum PhysicalProperties;
	class Polygon;
	class Layer;
}

// Required Definitions for Struct/Class Members
#include "Utils/MathTypes.h"
#include "Utils/Timestep.h"
#include "Utils/Color.h"
#include "Utils/rdt_string.h"
#include "Graphics/Texture/TextureAtlas.h"

/*
	Stringify name
*/
#define GET_NAME(name) #name

/*
	Adds this component member to the trace list, allowing it to
	be accessed in the editor. This should be called in the contstructor
	of a component child class.
*/
#define TRACE_COMPONENT_DATA2(ComponentName, MemberVariable) \
static bool is_ ## MemberVariable ## _defined = false; \
if(!is_ ## MemberVariable ## _defined) { \
	const char* memberName = GET_NAME(MemberVariable);\
	const char* componentName = typeid(*this).name();\
	core::SupportedTraceType type = core::ResolveSupportType(MemberVariable);\
	size_t offset = offsetof(ComponentName, MemberVariable);\
	DEFINE_MEMBER(componentName, memberName, type, offset);\
	is_ ## MemberVariable ## _defined = true;\
}

#define TRACE_COMPONENT_DATA3(ComponentName, MemberVariable, MemberVariableType) \
static bool is_ ## MemberVariable ## _defined = false; \
if(!is_ ## MemberVariable ## _defined) { \
	const char* memberName = GET_NAME(MemberVariable);\
	const char* componentName = typeid(*this).name();\
	core::SupportedTraceType type = core::ResolveSupportTypeFromString(GET_NAME(MemberVariableType));\
	size_t offset = offsetof(ComponentName, MemberVariable);\
	DEFINE_MEMBER(componentName, memberName, type, offset);\
	is_ ## MemberVariable ## _defined = true;\
}

#define EXPAND(x) x
#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define TRACE_COMPONENT_DATA(...) EXPAND(GET_MACRO(__VA_ARGS__, TRACE_COMPONENT_DATA3, TRACE_COMPONENT_DATA2)(__VA_ARGS__))

namespace rdt {

	// STL dll-interfaces
	EXPORT_STL(std::shared_ptr<Polygon>);

	namespace core {

		enum SupportedTraceType {
			SupportedTraceType_rdt_string,
			SupportedTraceType_layerPtr,
			SupportedTraceType_uint,
			SupportedTraceType_double,
			SupportedTraceType_float,
			SupportedTraceType_color,
			SupportedTraceType_polygon,
			SupportedTraceType_vec2d,
			SupportedTraceType_angle,
			SupportedTraceType_colliderID,
			SupportedTraceType_modelID,
			SupportedTraceType_textureID,
			SupportedTraceType_bool,
			SupportedTraceType_NotSupported,
		};

		SupportedTraceType ResolveSupportTypeFromString(const std::string& type);

		template<typename T>
		SupportedTraceType ResolveSupportType(const T& queryType)
		{
			if (typeid(queryType) == typeid(rdt_string)) {
				return SupportedTraceType_rdt_string;
			}

			if (typeid(queryType) == typeid(Layer*)) {
				return SupportedTraceType_layerPtr;
			}

			if (typeid(queryType) == typeid(unsigned int)) {
				return SupportedTraceType_uint;
			}

			if (typeid(queryType) == typeid(rdt::Color)) {
				return SupportedTraceType_color;
			}

			if (typeid(queryType) == typeid(std::shared_ptr<rdt::Polygon>)) {
				return SupportedTraceType_polygon;
			}

			if (typeid(queryType) == typeid(double)) {
				return SupportedTraceType_double;
			}

			if (typeid(queryType) == typeid(Vec2d)) {
				return SupportedTraceType_vec2d;
			}

			if (typeid(queryType) == typeid(float)) {
				return SupportedTraceType_float;
			}

			if (typeid(queryType) == typeid(Angle)) {
				return SupportedTraceType_angle;
			}

			if (typeid(queryType) == typeid(bool)) {
				return SupportedTraceType_bool;
			}

			return SupportedTraceType_NotSupported;
		}

		struct TraceData {
			const char* name;
			SupportedTraceType type;
			size_t offset;
		};
		
		/*
			This should be hidden from client, used only by the editor.
		*/
		struct ComponentTraceTracker {
			static std::unordered_map<std::string, std::vector<core::TraceData>> ComponentDefinitions;
			static void AddDefinition(const char* component, const char* memberName, core::SupportedTraceType type, size_t offset);
			
			/*
				Gets the member data of this component that have been marked for tracing
			*/
			static std::vector<core::TraceData>& GetTraceData(const char* ComponentName);
		};
	}


	struct RADIANT_API ECSComponent
	{
	protected:
		/*
			DO NOT CALL THIS! Call TRACE_COMPONENT_DATA(Component, Member) instead.
		*/
		void DEFINE_MEMBER(const char* component, const char* memberName, core::SupportedTraceType type, size_t offset);

	public:

	};

	/*
		Defines engine-specific configurations for an Entity,
		this component is highly coupled with the EntityManager
	*/
	struct RADIANT_API EntityConfig : ECSComponent
	{
		rdt_string alias;
		Layer* owner;

		EntityConfig();
	};
	
	/*
		For entity debugging, highly used by the editor for
		debug tracing and functionality. DebugComponent is
		stripped away on release.
	*/
	struct RADIANT_API DebugComponent : ECSComponent
	{
		bool show_collider_hitbox;

		DebugComponent();
	};

	/*
		Defines the dimensionality of an entity, that is
		its vertices, location in space and volume.
	*/
	struct RADIANT_API Sprite : ECSComponent
	{
		ModelID model;

		Sprite();

		/*
			Sets the sprite's modelID by querying the ModelManager
		*/
		void SetModel(const std::string& modelName);
	};

	// defines the locality and dimensions of an entity
	struct RADIANT_API Transform : ECSComponent
	{
		Vec2d position;
		Angle rotation;
		Vec2d scale;

		Transform();

		void Translate(float deltaTime, const Vec2d velocity);
	};

	/*
		Defines a physics object's properties
	*/
	struct RADIANT_API RigidBody2D : ECSComponent
	{
		RealmID realmID;
		ColliderID colliderID;
		PhysicalProperties physicalProperties;

		bool use_gravity;
		Vec2d velocity;

		Vec2d max_velocity;
		Vec2d acceleration;

		double mass;

		RigidBody2D();

		/*
			Updates the velocity vector by the given timestep, applies
			changes in velocity with any provided external forces.
		*/
		void UpdateVelocity(float deltaTime, Vec2d externalForces = Vec2d::Zero());
		
		/*
			Gets the dx/dy of the given deltaTime, based on velocity.
		*/
		Vec2d GetChangeInPosition(const float deltaTime);

		/*
			Returns true if the provided physical property matches the
			physical properties of this rigidbody in any way.
		*/
		bool HasProperties(PhysicalProperties propertyQuery);
	};

	/*
		Defines a renderable object
	*/
	struct RADIANT_API Renderable : ECSComponent
	{
		unsigned int layer;		    // The render layer to begin draw
		TextureID texture;		    // The texture to use
		bool flipTexture;			// If texture should be flipped
		AtlasProfile atlasProfile;	// the texture coords to use
		Color fillColor;			// The shader color for the polygon

		Renderable();
	};

	/*
		Defines the animation of a sprite. Animators refernece
		animation objects, which utilize a texture. That texture
		will override a renderable component's texture.
	*/
	struct RADIANT_API Animator : ECSComponent
	{
		AnimationID animationID;		// What animation to use
		AnimationIndex currentFrame;	// What frame in animation to use
		Timer timer;					// time left until next frame

		Animator();
	};
}