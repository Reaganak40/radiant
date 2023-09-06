#pragma once
#include "pch.h"

namespace Radiant {
	typedef enum GeoMode {
		FillMode = 1,
		OutlineMode = 2
	};

	/*
		Sets the geometry shader.
	*/
	void ActivateGeoMode(GeoMode mode);
}