#include "pch.h"
#include "GameState.h"

namespace rdt {

	struct GameState::Impl {
		BitSet m_states;
		unsigned int m_stateCount;

		Impl()
			: m_states(0), m_stateCount(0)
		{
		}

		~Impl()
		{
		}

		void Set(unsigned int stateID, bool nVal)
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
	};

	GameState::GameState()
		: m_impl(new GameState::Impl)
	{
	}
	GameState::~GameState()
	{
		delete m_impl;
	}
	void GameState::SetStateCount(unsigned int numStates)
	{
		m_impl->m_stateCount = numStates;
		m_impl->m_states.SetNewFlagMax(numStates);
	}
	void GameState::Set(unsigned int stateID, bool nVal)
	{
		m_impl->Set(stateID, nVal);
	}

	bool GameState::Check(unsigned int stateID)
	{
		return m_impl->m_states.CheckFlag(stateID);
	}
}