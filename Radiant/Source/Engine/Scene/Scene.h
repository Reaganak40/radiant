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
	using RealmID = unsigned int;
	using SceneID = unsigned int;
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
			Returns the global identifier for this scene, registered
			by the SceneManager. If this scene has no registered ID,
			returns 0.
		*/
		SceneID GetID();

		/*
			Returns the registered name for this scene.
		*/
		const char* GetName();

		/*
			Activates this scene and its elements, making
			it the current scene.

			Will call OnAttach() for layers and activate registered realms
		*/
		void Bind();

		/*
			Releases, unbinds, this scene, removing it from
			the current scene.

			Will call OnDetach() for layers and deactivate registered realms
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

		/*
			Returns the current list of realms owned by this scene.
		*/
		const std::vector<RealmID>& GetRealms() const;

		/*
			Retursn the current list of layers attached to this scene.
		*/
		const std::vector<std::shared_ptr<Layer>>& GetLayers() const;

		friend class SceneManager;

	protected:
		/*
			Adds a layer to the top of the layer stack.
		*/
		void AddLayer(std::shared_ptr<Layer> layer);


		/*
			Using the Physics API, creates a new realm and stores
			it in the realm list for this scene. Layers will have
			access to this.
		*/
		RealmID AddRealm();

		/*
			To implement function that is called before Bind(), allowing
			child scenes to add additional functionality before binding
			a scene.
		*/
		virtual void OnBind() {}

		/*
			To implement function that is called before Release(), allowing
			child scenes to add additional functionality before releasing a
			scene.
		*/
		virtual void OnRelease() {}


	private:
		/*
			Sets the global identfier for this scene instance
		*/
		void SetSceneID(SceneID nID);

		/*
			Sets the alias for the scene
		*/
		void SetSceneName(const std::string& name);
	};


}