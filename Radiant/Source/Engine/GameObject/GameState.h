#pragma once
#include "Utils/BitSet.h"

namespace rdt {
	class GameState {
	private:
		BitSet m_states;
		unsigned int m_stateCount;
	public:
		GameState();
		~GameState();

		/*
			Set the maximum number of game states.
		*/
		void SetStateCount(unsigned int numStates);

		/*
			Using a unique state ID, set and store its value in this game state container.
		*/
		void SetState(unsigned int stateID, bool nVal);

		/*
			Returns true if the provided unique state ID stored in this game state container is true.
		*/
		bool CheckState(unsigned int stateID);
	};
}