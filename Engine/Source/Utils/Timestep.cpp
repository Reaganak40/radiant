#include "pch.h"
#include "Timestep.h"

namespace rdt {

	Timer::Timer(float secondInterval)
		: m_maxTime(secondInterval), m_is_running(false), m_timeLeft(0)
	{
	}

	Timer::~Timer()
	{
	}

	void Timer::Start()
	{
		m_timeLeft = m_maxTime;
		m_is_running = m_timeLeft > 0;
	}

	bool Timer::Update(float deltaTime)
	{
		if (!m_is_running) {
			return false;
		}

		m_timeLeft -= deltaTime;

		if (m_timeLeft <= 0) {
			m_timeLeft = 0;
			m_is_running = false;
			return true;
		}
		return false;
	}
	void Timer::End()
	{
		m_is_running = false;
	}

	bool Timer::IsRunning()
	{
		return m_is_running;
	}

	void Timer::SetInterval(float nSecondInterval) {
		m_maxTime = nSecondInterval;
	}

	float Timer::GetTimeLeft() {
		return m_timeLeft;
	}

}
