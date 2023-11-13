#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	using UniqueID = unsigned int;
	class Scene;
}

// For ApplicationConfig Struct
#include "Utils/Color.h"
#include "Graphics/Camera.h"

namespace rdt {

	enum LoopPhase {
		LoopPhase_Begin,
		LoopPhase_ProcessInput,
		LoopPhase_WorldUpdate,
		LoopPhase_FinalUpdate,
		LoopPhase_RenderUpdate,
		LoopPhase_End,
	};

	class RADIANT_API Application
	{
	private:
		struct Impl;
		Impl* m_impl;

	public:
		Application();
		virtual ~Application();

		/*
			Starts a new Radiant application, launching the window instance. Aspect ratio is 16:9 by default.
			Window is resizable by default.
		*/
		void OnStart();

		/*
			Runs and manages the game loop, and will continue to run until the application is closed.
		*/
		void Run();

		/*
			Gets this frame's deltaTime.
		*/
		const float GetDeltaTime();

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
		const UniqueID AddScene(const std::string& sceneName, std::shared_ptr<Scene> nScene);

		/*
			Manually sets the scene in the application to the one that
			owns this uniqueID. This will call the Scene's OnBind() before
			it gets set and the current scene's OnRelease.
		*/
		void SetScene(const std::string& sceneName);

	protected:

		struct ApplicationConfig {
			std::string appName = "Radiant App";
			unsigned int windowWidth = 1280;
			unsigned int windowHeight = 720;
			AspectRatio cameraAspectRatio = AR_16_9;
			Color backgroundColor = WHITE;
		};

		/*
			Uses the struct to provide all application configuration
			that is used when the Application calls OnStart().
		*/
		void SetApplicationConfig(const ApplicationConfig& config);

		/*
			Called right before the game loop begins to define and setup scenes.
		*/
		virtual void OnGameBegin() {}

	private:

		/*
			Initializes the entity component system and common components
		*/
		void AddECS();

		/*
			Removes the entity component system singletons
		*/
		void RemoveECS();

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
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}