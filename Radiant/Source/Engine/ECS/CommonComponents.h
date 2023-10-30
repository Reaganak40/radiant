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
		Defines the dimensionality of an entity, that is
		its vertices, location in space and volume.
	*/
	struct Sprite
	{
		// Contains a centeralized origin and 2D vertices
		// NOTE: Pointer required because the number of vertices in arbitrary
		std::shared_ptr<Polygon> polygon = nullptr; 
	};

	/*
		Defines a physics object's properties
	*/
	struct RigidBody2D
	{
		double mass = 1;
	};

	/*
		Defines a renderable object
	*/
	struct Renderable
	{
		unsigned int layer = 0;		 // The render layer to begin draw
		std::string texture = "";    // texture alias to be applied to this render object
		Color polygon_color = BLACK; // The shader color for the polygon
	};
}