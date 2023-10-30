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

namespace rdt {

	/*
		Defines a physics object's shape and properties

		Contains a polygon which contains a centeralized origin and
		2D vertices. Used by the physics and rendering engines.
	*/
	struct RigidBody2D
	{
		std::shared_ptr<Polygon> polygon; // Contains a centeralized origin and 2D vertices
	};

	/*
		Defines a renderable object
	*/
	struct Renderable
	{
		unsigned int layer = 0;		 // The render layer to begin draw
		std::string texture = "";    // texture alias to be applied to this render object
		std::shared_ptr<Polygon> polygon = nullptr;  // Polygon to use for mesh vertices
		Color polygon_color = BLACK; // The shader color for the polygon

		bool IsUsingTexture() { return !texture.empty(); }
		bool IsUsingPolygon() { return polygon != nullptr; }

		/*
			Gets the polygon from the RigidBody component for this 
			entity and sets the pointer of the polygon attribute to it
		*/
		static void RADIANT_API AddPolygonFromRigidBody(Entity entity);
	};
}