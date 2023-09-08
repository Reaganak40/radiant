#include "pch.h"
#include "Timestep.h"

namespace Radiant {

	Timer::Timer(float secondInterval)
		: m_maxTime(secondInterval), m_is_running(false)
	{
	}

	Timer::~Timer()
	{
	}

	void Timer::Start()
	{
		m_timeLeft = m_maxTime;
		m_is_running = true;
	}

	bool Timer::Update(float deltaTime)
	{
		if (!m_is_running) {
			return false;
		}

		m_timeLeft -= deltaTime;

		return m_timeLeft < 0;
	}
	void Timer::End()
	{
		m_is_running = false;
	}

	bool Timer::IsRunning()
	{
		return m_is_running;
	}
}
