#pragma once
#include "Utils/MathTypes.h"
#include "Polygon/Polygon.h"
/*
	Common component data structs that are used by the engine,
	systems, and child applications.
*/

namespace rdt {

	/*
		Contains a polygon which contains a centeralized origin and
		2D vertices. Used by the physics and rendering engines.
	*/
	struct Transform2D
	{
		Polygon polygon;
	};
}