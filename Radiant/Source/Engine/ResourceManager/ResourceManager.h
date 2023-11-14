/*******************************************************************
*	Module:  Graphics (core)
*	File:    Animation.h
*
*	Author: Reagan Kelley
*
*   The Resource Manager is tasked with loading, tracking, and
*	removing resources from the engine.
*******************************************************************/
#pragma once

namespace rdt {

	class ResourceManager {
	private:
		struct Impl;
		static Impl* m_impl;
	public:

		static void Initialize();
		static void Destroy();

		static void LoadDefaultResources();
	};
}