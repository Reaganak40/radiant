#pragma once
#include "utils_api.h"

namespace rdt {
	class UTILS_API Timestep {
	private:
		unsigned long long _lastTimeFrame;
	public:
		float deltaTime;

		Timestep();

		const float GetSeconds() {
			return deltaTime;
		}

		const float GetMilliseconds() {
			return deltaTime * 1000.0f;
		}

		void Update(bool applyLimit = false);
	};

	class UTILS_API Timer {
	private:
		float m_maxTime;
		float m_timeLeft;
		bool m_is_running;

	public:
		Timer(float secondInterval = 1.0);
		~Timer();

		/*
			(Re)starts the timer.
		*/
		void Start();

		/*
			Reduces the time left on the timer by deltatime. Returns true
			if the timer has hit 0.
		*/
		bool Update(float deltaTime);

		/*
			Stops the timer.
		*/
		void End();

		/*
			Returns true if the timer was started
		*/
		bool IsRunning();

		/*
			Sets the internal interval limit.
		*/
		void SetInterval(float nSecondInterval);

		/*
			Get in seconds the time left on the timer.
		*/
		float GetTimeLeft();
	};
}