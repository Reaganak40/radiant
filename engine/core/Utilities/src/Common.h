/*******************************************************************
*	File: Common.h
*	Module: Utils
*
*	File contains many low-level enums and types that are used
*	in many different parts of the engine.
*******************************************************************/
#pragma once

namespace rdt {

	// Defines the aspect ratio of the game window
	enum AspectRatio {
		NoAspectRatio,  // No aspect ratio is maintained (stretch)
		AR_16_9,		// Window Projection = (1920px, 1080px)
		AR_1_1,			// Window Projection = (1000px, 1000px)
	};

	// Refers to what step in the game loop the engine is in
	enum LoopPhase {
		LoopPhase_Begin,
		LoopPhase_ProcessInput,
		LoopPhase_WorldUpdate,
		LoopPhase_FinalUpdate,
		LoopPhase_RenderUpdate,
		LoopPhase_End,
	};
}