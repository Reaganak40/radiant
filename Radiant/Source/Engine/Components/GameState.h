#pragma once
#include "Core.h"
#include "Utils/BitSet.h"

namespace rdt {
	class RADIANT_API GameState {
	private:
		struct Impl;
		Impl* m_impl;
	
	public:
		GameState();
		~GameState();

	protected:
		/*
			Set the maximum number of game states.
		*/
		void SetStateCount(unsigned int numStates);

		/*
			Using a unique state ID, set and store its value in this game state container.
		*/
		void Set(unsigned int stateID, bool nVal);

		/*
			Returns true if the provided unique state ID stored in this game state container is true.
		*/
		bool Check(unsigned int stateID);
	};
}