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
		struct Impl;
		Impl* m_impl;

	protected:
		/*
			Get the game objects registered at this layer.
		*/
		std::vector<GameObject*>& GetGameObjects();

		/*
			Get the GUIs registered at this layer.
		*/
		std::vector<GuiTemplate*>& GetGUIs();

		/*
			Registers a game object to this layer. The layer is now
			responsible for freeing this game object instance.
		*/
		void RegisterGameObject(GameObject* nGameObject);

		/*
			Registers a GUI to this layer. The layer is now
			responsible for freeing this pointer.
		*/
		void RegisterGUI(GuiTemplate* nGUI);


	public:
		Layer();
		~Layer();

		const UniqueID GetID();

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
		virtual void OnProcessInput(const float deltaTime);

		/*
			Function called when the layer is active and the host application
			calls FinalUpdate()
		*/
		virtual void OnFinalUpdate();

		/*
			Function called when the layer is active and the host application
			calls Render()
		*/
		virtual void OnRender();

		/*
			Returns true if the layer is attach flag is true.
		*/
		bool IsAttached();

		/*
			To implement function when the object receives a message from the Message Bus.
		*/
		void OnMessage(Message msg) override {}

		/*
			Returns a constant pointer to the array of game objects
		*/
		GameObject** GetGameObjects(size_t* numObjects);

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