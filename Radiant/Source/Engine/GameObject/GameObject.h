#pragma once
#include "Core.h"
#include "Utils/UniqueID.h"
#include "Messaging/MessageBus.h"
#include "Utils/BitSet.h"
#include "Polygon/Polygon.h"
#include "GameState.h"

namespace rdt {
	class RADIANT_API GameObject : public Messenger {
	private:
		struct Impl;
		Impl* m_impl;
		
	protected:

		/*
			Helper function for child game object classes to add a polygon
			to the physical world.
		*/
		void AddObjectToWorld(std::shared_ptr<Polygon> polygon);

	public:
		GameObject();
		~GameObject();

		/*
			Gets the Unique ID for this game object.
		*/
		const UniqueID GetID();

		/*
			Gets the scene this object belongs to.
		*/
		const UniqueID GetLayerID();

		/*
			Gets the realm this object belongs to.
		*/
		const UniqueID GetRealmID();

		/*
			Gets the ID to identify the physics object in the realm.
		*/
		const UniqueID GetModelID();

		/*
			To implement function when the layer this object belongs to is
			being binded.
		*/
		virtual void OnBind() {}

		/*
			To implement function when the scene this object belongs to is
			being released.
		*/
		virtual void OnRelease() {}

		/*
			To implement function when object needs to perform update procedures.
		*/
		virtual void OnProcessInput(const float deltaTime) {}

		/*
			To implement function when object needs to perform final update procedures.
		*/
		virtual void OnFinalUpdate() {}
		
		/*
			To implement function when object needs to perform rendering procedures.
		*/
		virtual void OnRender() {}

		/*
			To implement function when the object receives a message from the Message Bus.
		*/
		virtual void OnMessage(Message msg) override;


		// Functions to be called by Layer

		/*
			Binds this game object to an existing scene. This must be done
			before calling OnBind()
		*/
		void RegisterToLayer(const UniqueID nLayerID);

		/*
			Binds this game object to a realm, where it can communicate with the physics API
		*/
		void RegisterToRealm(const UniqueID nRealmID);


	};
}