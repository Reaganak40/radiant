#pragma once
#include "pch.h"

namespace Radiant {
	class Timestep {
	private:
		float _lastTimeFrame;
	public:
		float deltaTime;

		Timestep() : deltaTime(0), _lastTimeFrame(0) {}

		const float GetSeconds() {
			return deltaTime;
		}

		const float GetMilliseconds() {
			return deltaTime * 1000.0f;
		}

		void Update() {
			float time = (float)glfwGetTime();
			deltaTime = time - _lastTimeFrame;
			_lastTimeFrame = time;
		}
	};

	class Timer {
	private:
		float m_maxTime;
		float m_timeLeft;
		bool m_is_running;

	public:
		Timer(float secondInterval);
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
	};
}