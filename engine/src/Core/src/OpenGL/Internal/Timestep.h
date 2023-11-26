#pragma once

namespace rdt::glCore {
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

		void Update(bool applyLimit = false);
	};

}