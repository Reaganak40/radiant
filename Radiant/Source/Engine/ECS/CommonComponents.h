#pragma once
#include "Core.h"
#include "Utils/MathTypes.h"
#include "Utils/Color.h"
#include "Polygon/Polygon.h"
#include "ECSTypes.h"
/*
	Common component data structs that are used by the engine,
	systems, and child applications.
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

	class Layer;

	namespace core {

		enum SupportedTraceType {
			SupportedTraceType_stdstring,
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
			if (typeid(queryType) == typeid(std::string)) {
				return SupportedTraceType_stdstring;
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
	struct EntityConfig : ECSComponent
	{
		std::string alias = "";
		Layer* owner = nullptr;


		EntityConfig()
		{
			TRACE_COMPONENT_DATA(EntityConfig, alias);
			TRACE_COMPONENT_DATA(EntityConfig, owner);
		}
	};

	/*
		Defines the dimensionality of an entity, that is
		its vertices, location in space and volume.
	*/
	struct Sprite : ECSComponent
	{
		// Contains a centeralized origin and 2D vertices
		// NOTE: Pointer required because the number of vertices in arbitrary
		std::shared_ptr<Polygon> polygon = nullptr; 

		Sprite()
		{
			TRACE_COMPONENT_DATA(Sprite, polygon);
		}
	};

	/*
		Defines a physics object's properties
	*/
	struct RigidBody2D : ECSComponent
	{
		double mass = 1;

		RigidBody2D()
		{
			TRACE_COMPONENT_DATA(RigidBody2D, mass);
		}
	};

	/*
		Defines a renderable object
	*/
	struct Renderable : ECSComponent
	{
		unsigned int layer = 0;		 // The render layer to begin draw
		std::string texture = "";    // texture alias to be applied to this render object
		Color polygon_color = BLACK; // The shader color for the polygon

		Renderable()
		{
			TRACE_COMPONENT_DATA(Renderable, polygon_color);
			TRACE_COMPONENT_DATA(Renderable, layer);
			TRACE_COMPONENT_DATA(Renderable, texture);
		}
	};
}