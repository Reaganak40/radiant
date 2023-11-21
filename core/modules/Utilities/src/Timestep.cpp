#include "pch.h"
#include "Timestep.h"
#include "UtilFuncs.h"

namespace rdt {

	Timestep::Timestep()
		: deltaTime(0)
	{
		_lastTimeFrame = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	}

	void Timestep::Update(bool applyLimit)
	{
		unsigned long long time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		deltaTime = (time - _lastTimeFrame) / 1000.0f;
		_lastTimeFrame = time;

		if (applyLimit) {
			deltaTime = utils::Min(deltaTime, 0.03334f);
		}
	}

	// ========================================================

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
