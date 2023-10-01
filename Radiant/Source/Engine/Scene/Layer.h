#pragma once
#include "Messaging/Messenger.h"
#include "Utils/UniqueID.h"
#include "GameObject/GameObject.h"
#include "Gui/Gui.h"


namespace rdt {
	
	class Scene;

	/*
		Scenes own game objects and GUIs that can be used by the application under
		specified conditions.


	*/
	class Layer : public Messenger {
	private:
		UniqueID m_ID;

	protected:
		std::vector<GameObject*> m_game_objects;
		std::vector<GuiTemplate*> m_GUIs;
		std::vector<UniqueID> m_realms;
		GameState GState;
		
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
			To implement function when the object receives a message from the Message Bus.
		*/
		void OnMessage(Message msg) override {}

	public:
		Layer();
		~Layer();

		const UniqueID GetID() { return m_ID; }

		/*
			Function called when this layer is first attached to a Scene.
		*/
		virtual void OnAttach() {}

		/*
			Function called when this layer is to be awakened and active.
		*/
		virtual void Awake() {}

		/*
			Function to be called when this layer is to go to sleep but no destroyed.
		*/
		virtual void Sleep() {}

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

	private:
	};
}