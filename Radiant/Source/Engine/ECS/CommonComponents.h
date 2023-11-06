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
	class Polygon;
	class Layer;
}

// Required Definitions for Struct/Class Members
#include "Utils/MathTypes.h"
#include "Utils/Color.h"
#include "Utils/rdt_string.h"

/*
	Stringify name
*/
#define GET_NAME(name) #name

/*
	Adds this component member to the trace list, allowing it to
	be accessed in the editor. This should be called in the contstructor
	of a component child class.
*/
#define TRACE_COMPONENT_DATA(ComponentName, MemberVariable) \
static bool is_ ## MemberVariable ## _defined = false; \
if(!is_ ## MemberVariable ## _defined) { \
	const char* memberName = GET_NAME(MemberVariable);\
	const char* componentName = typeid(*this).name();\
	core::SupportedTraceType type = core::ResolveSupportType(MemberVariable);\
	size_t offset = offsetof(ComponentName, MemberVariable);\
	DEFINE_MEMBER(componentName, memberName, type, offset);\
	is_ ## MemberVariable ## _defined = true;\
}

namespace rdt {

	// STL dll-interfaces
	EXPORT_STL(std::shared_ptr<Polygon>);

	namespace core {

		enum SupportedTraceType {
			SupportedTraceType_rdt_string,
			SupportedTraceType_layerPtr,
			SupportedTraceType_uint,
			SupportedTraceType_double,
			SupportedTraceType_color,
			SupportedTraceType_polygon,
			SupportedTraceType_NotSupported,
		};

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

			return SupportedTraceType_NotSupported;
		}

		struct TraceData {
			SupportedTraceType type;
			size_t offset;
		};
		
	}

	struct RADIANT_API ECSComponent
	{
	private:

		static std::unordered_map<std::string, std::unordered_map<std::string, core::TraceData>> ComponentDefinitions;

	protected:

		void DEFINE_MEMBER(const char* component, const char* memberName, core::SupportedTraceType type, size_t offset);

	public:

		/*
			Gets the member data of this component that have been marked for tracing
		*/
		static std::unordered_map<std::string, core::TraceData>& GetTraceData(const char* ComponentName);
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
		Defines the dimensionality of an entity, that is
		its vertices, location in space and volume.
	*/
	struct RADIANT_API Sprite : ECSComponent
	{
		// Contains a centeralized origin and 2D vertices
		// NOTE: Pointer required because the number of vertices in arbitrary
		std::shared_ptr<Polygon> polygon; 
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
		float rotation;
		Vec2d scale;
	};

	/*
		Defines a physics object's properties
	*/
	struct RADIANT_API RigidBody2D : ECSComponent
	{
		double mass;

		RigidBody2D();
	};

	/*
		Defines a renderable object
	*/
	struct RADIANT_API Renderable : ECSComponent
	{
		unsigned int layer;		// The render layer to begin draw
		rdt_string texture;     // texture alias to be applied to this render object
		Color polygon_color;    // The shader color for the polygon

		Renderable();
	};
}