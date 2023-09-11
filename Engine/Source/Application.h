#pragma once
#include "pch.h"
#include "Utils/Timestep.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

namespace rdt {
	class Application {
	private:
		Timestep m_timestep;
		Scene* m_current_scene;

	public:
		Application();
		~Application();

		/*
			Starts a new rdt application, launching the window instance.
			Aspect ratio is 16:9 by default.
			Window is resizable by default.
		*/
		void Start(std::string appName, unsigned int windowWidth = 1280, unsigned int windowHeight = 720, bool resizable=true);

		/*
			Returns true if the application is running and the window is launched.
		*/
		bool IsRunning();

		/*
			Start of game loop, clears the window, gets new deltaTime and prepares internals
			for following update procedures.
		*/
		void BeginFrame();

		/*
			Runs the OnProcessInput on game objects, which will result in requests
			made in game space and the physics API.
		*/
		void ProcessInput();

		/*
			Updated the physical world one timestep, triggers collisions, resolving them
			and sending messages back to game objects.
		*/
		void UpdateWorld();

		/*
			Runs the OnFinalUpdate() on all game objects for final changes before render.
		*/
		void FinalUpdate();

		/*
			Runs the renderer draw command queue, resulting in a new frame.
		*/
		void Render();

		/*
			Polls inputs and events, getting ready for next frame of the game loop.
		*/
		void EndFrame();

		/*
			Gets this frame's deltaTime.
		*/
		const float GetDeltaTime() { return m_timestep.deltaTime; }

		/*
			Get the width in pixels of the window.
		*/
		const int WindowWidth();

		/*
			Get the height in pixels of the window.
		*/
		const int WindowHeight();

		/*
			Registers a scene to the application called its OnRegister()
			and adding it to its list of scenes.
		*/
		const UniqueID AddScene(const std::string& sceneName, Scene* nScene);

		/*
			Manually sets the scene in the application to the one that
			owns this uniqueID. This will call the Scene's OnBind() before
			it gets set and the current scene's OnRelease.
		*/
		void SetScene(const std::string& sceneName);

	};
}