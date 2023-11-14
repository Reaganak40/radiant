/*******************************************************************
*	Module:  Graphics (API)
*	File:    Animation.h
*
*	Author: Reagan Kelley
*
*   Animation instances define a sequence of texture atlas frames
*	that when changed at a frequency produce animation effects
*	for a sprite.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	using TextureID = unsigned int;   // Unique Identifier for a Texture
	class TextureAtlas;
	struct AtlasProfile;
}

// Indicates no refernece to an animation
#define RDT_NULL_ANIMATION_ID 0

namespace rdt {

	using AnimationID = unsigned int;    // Unique Identifier for an Animation object.
	using AnimationIndex = unsigned int; // Indicates the index of an animation sequence

	class Animation {
	private:
		std::shared_ptr<TextureAtlas> m_atlas;
		std::vector<AtlasProfile> m_frames;

	public:
		Animation(TextureID textureWithAtlas);
		~Animation();

		/*
			Adds a frame to the animation, indicating the next
			texture sprite to show in the sequence. Will use
			the sprite at the given atlas coordinates.
		*/
		void PushFrame(unsigned int atlasX, unsigned int atlasY);

		/*
			Adds a frame to the animation, indicating the next
			texture sprite to show in the sequence. Will use
			the sprite at the given sprite indicator.
		*/
		void PushFrame(int spriteIndicator);

		/*
			Returns the atlas sprite profile defined by the texture
			atlas, at the given animation frame index.
		*/
		AtlasProfile GetFrameAt(AnimationIndex index);

		/*
			Returns the number of frames in this animation.
		*/
		size_t FrameCount();


		/*
			Gets the textureID that this animation is using.
		*/
		TextureID GetTextureID();
	};

	// ===========================================================

	class RADIANT_API AnimationManager
	{
	private:
		struct Impl;
		static Impl* m_impl;

	public:
		
		/*
			Creates a new context of the AnimationManager singleton
		*/
		static void Initialize();

		/*
			Destroys the current AnimationManager singleton instance.
		*/
		static void Destroy();

		/*
			Creates a new animation instance utlizing the texture atlas of the
			provided texture. 
		*/
		static AnimationID CreateNewAnimation(const std::string& name, TextureID texture);

		/*
			Gets an instantiated animation's ID, belonging to this name. Otherwise,
			returns 0.
		*/
		static AnimationID GetAnimationID(const std::string& name);

		

		/*
			Returns the textureID for the texture that is being used by the
			provided AniamtionID.
		*/
		static TextureID GetAnimationTexture(AnimationID aID);

		/*
			Gets the AtlasProfile (frame), for an animation at the given
			index. If animation is not found or index out of range, returns
			the entire texture (full texture coordinates).
		*/
		static AtlasProfile GetFrame(AnimationID aID, AnimationIndex aIndex);

		/*
			Returns the number of frames in an animation.
		*/
		static size_t GetFrameCount(AnimationID aID);

	private:
		/*
			Returns the shared pointer of an instantiated animation object.
		*/
		static std::shared_ptr<Animation> GetAnimation(AnimationID aID);
	};
}