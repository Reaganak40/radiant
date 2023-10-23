#pragma once
#include "Core.h"
#include "Components/Component.h"
#include "Utils/UniqueID.h"
#include "Polygon/Polygon.h"

namespace rdt {
	class RADIANT_API GameObject : public core::Component {
	private:
		struct Impl;
		Impl* m_impl;
		
	protected:

		/*
			Helper function for child game object classes to add a polygon
			to the physical world. When passing a null polygon, will try to
			use the cached (previously removed) object. 
		*/
		void AddObjectToWorld(std::shared_ptr<Polygon> polygon = nullptr);

		/*
			Helper function for child game object classes to remove a poylgon from the
			physical world. Also caches the polygon for re-introduction.
		*/
		void RemoveObjectFromWorld();

		/*
			Returns the spawn position of this object (if not assigned by the layer should be Zero())
		*/
		const Vec2d& GetSpawnPos();

		/*
			Returns true if the registered game object is in view of the camera
		*/
		bool IsObjectInView();

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
			Returns true if this game object is binded to a scene.
		*/
		const bool IsBinded();

		/*
			To implement function when the layer this object belongs to is
			being binded. The base class should be called at the end to set
			internal bind flags.
		*/
		virtual void OnBind();

		/*
			To implement function when the scene this object belongs to is
			being released. The base class should be called at the end to set
			internal bind flags.
		*/
		virtual void OnRelease();

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

		/*
			Sets the spawn position of this object.
		*/
		void SetSpawnPos(double xPos, double yPos);
	};
}