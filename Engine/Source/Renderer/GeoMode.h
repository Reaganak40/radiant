#pragma once
#include "pch.h"

namespace Radiant {
	enum GeoMode {
		FillMode = 1,
		OutlineMode = 2
	};

	/*
		Sets the geometry shader.
	*/
	void ActivateGeoMode(GeoMode mode);
}