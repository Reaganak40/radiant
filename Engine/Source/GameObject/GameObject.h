#pragma once
#include "Utils/UniqueID.h"

namespace Radiant {
	class GameObject {
	private:
		UniqueID m_ID;
		UniqueID m_sceneID;
		UniqueID m_realmID;

	protected:
		UniqueID m_model_ID;

	public:
		GameObject() : m_ID(GetUniqueID()), m_sceneID(0), m_model_ID(0), m_realmID(0) {}
		~GameObject() {}

		const UniqueID GetID() { return m_ID; }

		/*
			Binds this game object to an existing scene. This must be done
			before calling OnBind()
		*/
		void RegisterToScene(const UniqueID nSceneID) { m_sceneID = nSceneID; }

		/*
			Binds this game object to a realm, where it can communicate with the physics API
		*/
		void RegisterToRealm(const UniqueID nRealmID) { m_realmID = nRealmID; }

		/*
			Gets the scene this object belongs to.
		*/
		const UniqueID GetSceneID() { return m_sceneID; }

		/*
			Gets the realm this object belongs to.
		*/
		const UniqueID GetRealmID() { return m_realmID; }

		/*
			To implement function when the scene this object belongs to is
			being binded.
		*/
		virtual void OnBind() {}

		/*
			To implement function when object needs to perform update procedures.
		*/
		virtual void OnProcessInput(const float deltaTIme) {}

		/*
			To implement function when object needs to perform final update procedures.
		*/
		virtual void OnFinalUpdate() {}
		
		/*
			To implement function when object needs to perform rendering procedures.
		*/
		virtual void OnRender() {}

	};
}