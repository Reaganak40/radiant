#include "pch.h"
#include "Timestep.h"

#include <Radiant/Utils.h>

namespace rdt::glCore {

	void Timestep::Update(bool applyLimit)
	{
		float time = (float)glfwGetTime();
		deltaTime = time - _lastTimeFrame;
		_lastTimeFrame = time;

		if (applyLimit) {
			deltaTime = utils::Min(deltaTime, 0.03334f);
		}
	}
}
