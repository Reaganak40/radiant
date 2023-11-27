/*******************************************************************
*	Module:  Vertex
*
*	This section contains the Vertex struct, which contains all the
*	data for a single OpenGL vertex point. Used by the client
*	to define geometry, and used throughout the internal rendering
*	pipeline.
*******************************************************************/
#pragma once
#include "gl_core_api.h"

namespace rdt::glCore {
	struct GL_CORE_API glVertex {
		float position	[3];	// x,y,z coordinats of a vertex
		float color		[4];	// the rgba (normalized) color values of a vertex
		float texCoords	[2];	// the texture mapping coordinates for this vertex
		float texIndex;         // the texture slot that the target texture is currently binded.

		glVertex();

		void SetPosition(const float nPosition[3]);
		void SetPosition(const float x, const float y, const float z);

		void SetColor(const float nColor[4]);
		void SetColor(const float r, const float g, const float b, const float a);

		void SetTexCoords(const float nTexCoords[2]);
		void SetTexCoords(const float x, const float y);

		void SetTexIndex(float nIndex);
	};
}
