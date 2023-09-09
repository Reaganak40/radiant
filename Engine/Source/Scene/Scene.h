#pragma once
#include "Utils/UniqueID.h"
#include "GameObject/GameObject.h"
#include "Gui/Gui.h"

namespace Radiant {
	
	/*
		Scenes own game objects and GUIs that can be used by the application under
		specified conditions.


	*/
	class Scene {
	private:
		UniqueID m_ID;

	protected:
		std::vector<GameObject*> m_game_objects;
		std::vector<GuiTemplate*> m_GUIs;
		std::vector<UniqueID> m_realms;
		
		/*
			Runs OnProcssInput on all game objects and OnUpdate on all
			GUIs. This should be called at the end of OnProcessInput().
		*/
		void RunProcessInputQueue(const float deltaTime);

		/*
			Runs OnFinalUpdate() on all game objects. This should be called
			at the end of OnFinalUpdate().
		*/
		void RunFinalUpdateQueue();

		/*
			Runs OnRender() on all game objects. This should be called at the end
			of OnRender().
		*/
		void RunRenderQueue();

		/*
			Changes the scene dynamically to a known scene. This change is delayed
			until the start of the next game loop.
		*/
		void ChangeScene(const std::string& nScene);

	public:
		Scene();
		~Scene();

		const UniqueID GetID() { return m_ID; }

		/*
			Function called prior to the host application registering
			the scene with the SceneManager for the first time. This is where
			permament game objects should be created.
		*/
		virtual void OnRegister() {}

		/*
			Function called prior to when the application switches to the
			scene.
		*/
		virtual void OnBind() {}

		/*
			Function called when a scene is about to be released, changed
			to another scene.
		*/
		virtual void OnRelease() {}

		/*
			Function called when the scene is active and the host application
			calls ProcessInput.
		*/
		virtual void OnProcessInput(const float deltaTime) { RunProcessInputQueue(deltaTime); }

		/*
			Function called when the scene is active and the host application
			calls FinalUpdate()
		*/
		virtual void OnFinalUpdate() { RunFinalUpdateQueue(); }

		/*
			Function called when the scene is active and the host application
			calls Render()
		*/
		virtual void OnRender() { RunRenderQueue(); }

	private:
	};
}