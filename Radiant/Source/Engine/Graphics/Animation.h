/*******************************************************************
*	Module:  Graphics
*	File:    Animation.h
*
*	Author: Reagan Kelley
*
*   Animation instances define a sequence of texture atlas frames
*	that when changed at a frequency produce animation effects
*	for a sprite.
*******************************************************************/
#pragma once

// Forward Declarations
namespace rdt {
	using TextureID = unsigned int;   // Unique Identifier for a Texture
}

namespace rdt {
	class Animation {
	private:
		TextureID texture;
	};
}