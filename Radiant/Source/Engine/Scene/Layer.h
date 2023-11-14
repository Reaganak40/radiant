/*******************************************************************
*	Module:  Scene (API)
*	File:    Layer.h
*
*	Author: Reagan Kelley
*
*	In this file contains the definition for Layer, a core
*	component that holds entity data and game logic. Child
*	layers makeup most of the logic of a game application.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	class EntityDefinition;
	using RealmID = unsigned int;
	using Entity = std::uint32_t;
	using ChannelID = unsigned int;
	using LayerID = unsigned int;
	struct Message;
}

namespace rdt {
	
	class Scene;

	/*
		Scenes own game objects and GUIs that can be used by the application under
		specified conditions.
	*/
	class RADIANT_API Layer {
	private:
		struct Impl;
		Impl* m_impl;

	protected:
		/*
			Registers a new entity to this layer. The layer is now
			responsible for this entity. Returns the entity's ID

			Can provide an alias to this entity, otherwise it will be given a default name.
			
			NOTE: This entity definition will be freed upon completion of this function.
		*/
		Entity RegisterEntity(EntityDefinition* nEntity, const std::string& alias = "");

	public:
		Layer();
		~Layer();

		/*
			Called after a layer has been initialized and appriopriately
			configured and binded to its host scene. It is recomended that
			this is used to register entities.
		*/
		virtual void OnCreate() {}

		/*
			Provides to the layer an internal reference to its parent
			scene. Uses to query scene related resources.
		*/
		void BindToScene(Scene* scene);

		/*
			Gets the ID for this layer. Returns 0 if this layer
			has no registered ID.
		*/
		LayerID GetID();

		/*
			Function called when the host scene of this layer is binded, entering
			the game loop.
		*/
		virtual void OnAttach() {}

		/*
			Function called when the host scene of this layer is about to be released, not active,
			removed from the game loop.
		*/
		virtual void OnDetach() {}

		/*
			Returns true if the layer is attach flag is true.
		*/
		bool IsAttached();

		/*
			Returns a list of the entities registered by this layer.
		*/
		const std::vector<Entity>& GetEntities();

		/*
			Runs the ProcessInput game-loop-step on this layer.
		*/
		void ProcessInput(const float deltaTime);

		/*
			Runs the UpdateWorld game-loop-step on this layer.
		*/
		void UpdateWorld(const float deltaTime);

		/*
			Runs the FinalUpdate game-loop-step on this layer.
		*/
		void FinalUpdate();

		/*
			Runs the RenderUpdate game-loop-step on this layer.
		*/
		void RenderUpdate();

		/*
			Polls the message bus for messages from the subscribed channels
		*/
		void PollMessages();

		friend class SceneManager;
		
	protected:

		/*
			Tells the layer's communicator to poll messages from this channel
		*/
		void SubscribeToChannel(ChannelID channel);

		/*
			Gets the current message channel for a message poll. If not
			currently polling messages, returns 0.
		*/
		ChannelID CurrentMessageChannel();

		/*
			To implement function when the object receives a message from the Message Bus.
		*/
		virtual void OnMessage(const Message& msg) {}

		/*
			To implement function, called right before the base class
			calls ProcessInput.
		*/
		virtual void OnProcessInput(const float deltaTime) {}

		/*
			To implement function, called right before the base class
			calls UpdateWorld.
		*/
		virtual void OnUpdateWorld(const float deltaTime) {}

		/*
			To implement function, called right before the base class
			calls FinalUpdate.
		*/
		virtual void OnFinalUpdate() {}

		/*
			To implement function, called right before the base class
			calls RenderUpdate.
		*/
		virtual void OnRenderUpdate() {}

		/*
			Gets the host scene's registered realm at the 
			given index. Returns 0 if no such realm exists.
		*/
		RealmID GetRealm(size_t realmIndex = 0);

	private:

		/*
			Overhead function to set the attach flag for the layer.
		*/
		void SetAttached(bool attach);

		/*
			Provided this layer with a global identifer, registered
			by the SceneManager.
		*/
		void SetLayerID(LayerID nID);
	};
}