/*******************************************************************
*	Module:  Scene (API)
*	File:    Scene.h
*
*	Author: Reagan Kelley
*
*	Scenes act at logic containers for layers. They represent
*	the game state. Only one scene is active at a time.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	class Layer;
}

namespace rdt {

	class RADIANT_API Scene {
	private:
		struct Impl;
		Impl* m_impl;

	public:
		Scene();
		~Scene();

		/*
			Adds a layer to the top of the layer stack. The scene is
			now responsible for freeing this layer.
		*/
		void AddLayer(std::shared_ptr<Layer> nLayer);

		/*
			Activates this scene and its elements, making
			it the current scene.
		*/
		void Bind();

		/*
			Releases, unbinds, this scene, removing it from
			the current scene.
		*/
		void Release();

		/*
			Runs the ProcessInput game-loop-step on this scene.
		*/
		void ProcessInput(const float deltaTime);

		/*
			Runs the UpdateWorld game-loop-step on this scene.
		*/
		void UpdateWorld(const float deltaTime);

		/*
			Runs the FinalUpdate game-loop-step on this scene.
		*/
		void FinalUpdate();

		/*
			Runs the RenderUpdate game-loop-step on this scene.
		*/
		void RenderUpdate();
	};
}