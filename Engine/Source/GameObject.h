#pragma once
#include "Utils/UniqueID.h"

namespace Radiant {
	class GameObject {
	private:
		UniqueID m_UUID;
	protected:
		UniqueID m_model_ID;

	public:
		GameObject() : m_UUID(GetUniqueID()), m_model_ID(0) {}
		~GameObject() {}

		const UniqueID GetUUID() { return m_UUID; }

		/*
			To implement function when object is being registered as a game object
			with the game application.
		*/
		virtual void OnRegister() {}

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