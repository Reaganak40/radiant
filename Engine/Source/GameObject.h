#pragma once
#include "Utils/UniqueID.h"

namespace Radiant {
	class GameObject {
	private:
		UniqueID m_UUID;
	public:
		GameObject() : m_UUID(GetUniqueID()) {}
		~GameObject() {}

		const UniqueID GetUUID() { return m_UUID; }

		/*
			To implement function when object needs to perform update procedures.
		*/
		virtual void OnUpdate(const float deltaTIme) {}
		
		/*
			To implement function when object needs to perform rendering procedures.
		*/
		virtual void OnRender() {}

		/*
			To implement function when object needs to perform end of frame procedures.
		*/
		virtual void OnEndFrame() {}

	};
}