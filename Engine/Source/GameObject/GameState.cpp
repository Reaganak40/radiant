#include "pch.h"
#include "GameState.h"

namespace rdt {
	GameState::GameState()
		: m_states(0), m_stateCount(0)
	{
	}
	GameState::~GameState()
	{
	}
	void GameState::SetStateCount(unsigned int numStates)
	{
		m_stateCount = numStates;
		m_states.SetNewFlagMax(numStates);
	}
	void GameState::SetState(unsigned int stateID, bool nVal)
	{
		if (stateID > m_stateCount) {
			return;
		}

		if (nVal) {
			m_states.ActivateFlag(stateID);
		}
		else {
			m_states.ActivateFlag(stateID);
			m_states.DeactivateFlag(stateID);
		}
	}
	bool GameState::CheckState(unsigned int stateID)
	{
		return m_states.CheckFlag(stateID);
	}
}