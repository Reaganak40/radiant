#pragma once
#include "pch.h"
#include "Utils/Timestep.h"
#include "GameObject.h"

namespace Radiant {
	class Application {
	private:
		Timestep m_timestep;
		std::vector<GameObject*> m_game_objects;

	public:
		Application();
		~Application();

		/*
			Starts a new Radiant application, launching the window instance.
		*/
		void Start(std::string appName, unsigned int windowWidth = 1280, unsigned int windowHeight = 720);

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
			Adds a game object to the world. Note: the application will now own this pointer
			and instance and is responsible for freeing it.
		*/
		const UniqueID AddGameObject(GameObject* nGameObject);

	};
}