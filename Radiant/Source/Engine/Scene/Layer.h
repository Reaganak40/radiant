#pragma once
#include "Core.h"
#include "Messaging/Messenger.h"
#include "Utils/UniqueID.h"
#include "GameObject/GameObject.h"
#include "Gui/Gui.h"
#include "Graphics/Camera.h"


namespace rdt {
	
	class Scene;

	/*
		Scenes own game objects and GUIs that can be used by the application under
		specified conditions.
	*/
	class RADIANT_API Layer : public Messenger {
	private:
		UniqueID m_ID;
		bool m_attached;

	protected:
		std::vector<GameObject*> m_game_objects;
		std::vector<GuiTemplate*> m_GUIs;
		std::vector<UniqueID> m_realms;
		GameState GState;

	private:
		
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

	public:
		Layer();
		~Layer();

		const UniqueID GetID() { return m_ID; }

		/*
			Function called when this layer is binded to a Scene, entering
			the game loop.
		*/
		virtual void OnAttach() {}

		/*
			Function called when a layer is about to be released, not active.
		*/
		virtual void OnDetach() {}

		/*
			Function called when the layer is active and the host application
			calls ProcessInput.
		*/
		virtual void OnProcessInput(const float deltaTime) { RunProcessInputQueue(deltaTime); }

		/*
			Function called when the layer is active and the host application
			calls FinalUpdate()
		*/
		virtual void OnFinalUpdate() { RunFinalUpdateQueue(); }

		/*
			Function called when the layer is active and the host application
			calls Render()
		*/
		virtual void OnRender() { RunRenderQueue(); }

		/*
			Returns true if the layer is attach flag is true.
		*/
		bool IsAttached() { return m_attached; }

		/*
			To implement function when the object receives a message from the Message Bus.
		*/
		void OnMessage(Message msg) override {}

		/*
			Returns a constant pointer to the array of game objects
		*/
		GameObject** GetGameObjects(unsigned int* numObjects);

		friend class Scene;

	protected:

		/*
			Helper function that binds all game objects and GUIs
		*/
		void BindAll();

		/*
			Helper function that unbinds all game objects and detaches all GUIs.
		*/
		void ReleaseAll();

		/*
			Initalized a new realm from the physics engine and appends it to the vector
			of realms to be used by game objects.
		*/
		void CreateNewRealm();

	private:

		/*
			Overhead function to set the attach flag for the layer.
		*/
		void SetAttached(bool attach);
	};
}