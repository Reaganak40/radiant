#pragma once
#include "Utils/UniqueID.h"
#include "Messaging/Messenger.h"
#include "Layer.h"

namespace rdt {

	class Scene : public Messenger {
	private:
		UniqueID m_ID;
		std::vector<Layer*> m_layers;

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

	protected:


	public:
		Scene();
		~Scene();

		const UniqueID GetID() { return m_ID; }

		/*
			Changes the scene dynamically to a known scene. This change is delayed
			until the start of the next game loop.
		*/
		void ChangeScene(const std::string& nScene);

		/*
			Adds a layer to the top of the layer stack. The scene is
			now responsible for freeing this layer.
		*/
		void AttachLayer(Layer* nLayer);

		/*
			Function called prior to the host application registering
			the scene with the SceneManager for the first time.
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
	};
}