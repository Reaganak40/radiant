#pragma once
#include "Core.h"
#include "Utils/UniqueID.h"
#include "Messaging/Messenger.h"
#include "Graphics/Camera.h"
#include "Layer.h"

namespace rdt {

	class RADIANT_API Scene : public Messenger {
	private:
		struct Impl;
		Impl* m_impl;

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
		
		/*
			When this function is called, the render call internals for this Scene instance will
			not tell the renderer to use the default camera, thus it is up to the scene child
			to choose the right camera manually.
		*/
		void DontUseDefaultCamera();
	public:
		Scene();
		~Scene();

		const UniqueID GetID();

		/*
			Adds a layer to the top of the layer stack. The scene is
			now responsible for freeing this layer. This layer is not
			attached.
		*/
		void AddLayer(Layer* nLayer);

		/*
			Function called prior to the host application registering
			the scene with the SceneManager for the first time.
		*/
		virtual void OnRegister() {}

		/*
			Function called prior to when the application switches to the
			scene.
		*/
		virtual void OnBind();

		/*
			Function called when a scene is about to be released, changed
			to another scene.
		*/
		virtual void OnRelease();

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

		/*
			Returns a constant pointer to the array of layers
		*/
		Layer** GetLayers(size_t* numLayers);
	};
}